import { useRef, useCallback } from 'react';
import { BANDS } from '../audio/bands';
import { applyState as engineApplyState, createFilterChain } from '../audio/AudioEngine';
import type { SemanticState } from '../audio/types';

export function useAudioEngine() {
  const ctxRef = useRef<AudioContext | null>(null);
  const filtersRef = useRef<BiquadFilterNode[]>([]);
  const analyserRef = useRef<AnalyserNode | null>(null);
  const inputAnalyserRef = useRef<AnalyserNode | null>(null);
  const sourceRef = useRef<AudioNode | null>(null);

  const initAudio = useCallback(() => {
    if (ctxRef.current) return;

    const ctx = new AudioContext();
    ctxRef.current = ctx;

    const filters = createFilterChain(ctx);
    filtersRef.current = filters;

    // Output analyser (post-EQ)
    const analyser = ctx.createAnalyser();
    analyser.fftSize = 4096;
    analyser.smoothingTimeConstant = 0.8;
    filters[filters.length - 1].connect(analyser);
    analyser.connect(ctx.destination);
    analyserRef.current = analyser;

    // Input analyser (pre-EQ, for input meter)
    const inputAnalyser = ctx.createAnalyser();
    inputAnalyser.fftSize = 256;
    inputAnalyserRef.current = inputAnalyser;
  }, []);

  const connectSource = useCallback((node: AudioNode) => {
    if (sourceRef.current) {
      try { sourceRef.current.disconnect(); } catch { /* already disconnected */ }
    }
    sourceRef.current = node;
    if (inputAnalyserRef.current) {
      node.connect(inputAnalyserRef.current);
    }
    if (filtersRef.current.length > 0) {
      node.connect(filtersRef.current[0]);
    }
  }, []);

  const connectMic = useCallback(async (deviceId?: string) => {
    initAudio();
    const ctx = ctxRef.current!;
    if (ctx.state === 'suspended') await ctx.resume();

    const stream = await navigator.mediaDevices.getUserMedia({
      audio: {
        echoCancellation: false,
        noiseSuppression: false,
        autoGainControl: false,
        ...(deviceId ? { deviceId: { exact: deviceId } } : {}),
      },
    });
    const source = ctx.createMediaStreamSource(stream);
    connectSource(source);
    return 'mic';
  }, [initAudio, connectSource]);

  const connectFile = useCallback(async (file: File) => {
    initAudio();
    const ctx = ctxRef.current!;
    if (ctx.state === 'suspended') await ctx.resume();

    const arrayBuffer = await file.arrayBuffer();
    const audioBuffer = await ctx.decodeAudioData(arrayBuffer);

    // Stop previous buffer source if any
    if (sourceRef.current && 'stop' in sourceRef.current) {
      try { (sourceRef.current as AudioBufferSourceNode).stop(); } catch { /* already stopped */ }
    }

    const bufferSource = ctx.createBufferSource();
    bufferSource.buffer = audioBuffer;
    bufferSource.loop = true;
    connectSource(bufferSource);
    bufferSource.start();
    return file.name;
  }, [initAudio, connectSource]);

  const disconnectSource = useCallback(() => {
    if (!sourceRef.current) return;
    // Stop buffer sources
    if ('stop' in sourceRef.current) {
      try { (sourceRef.current as AudioBufferSourceNode).stop(); } catch { /* already stopped */ }
    }
    // Release mic hardware
    if ('mediaStream' in sourceRef.current) {
      (sourceRef.current as MediaStreamAudioSourceNode).mediaStream.getTracks().forEach(t => t.stop());
    }
    try { sourceRef.current.disconnect(); } catch { /* already disconnected */ }
    sourceRef.current = null;
  }, []);

  const applyState = useCallback((state: SemanticState) => {
    engineApplyState(state, filtersRef.current, BANDS);
  }, []);

  const toggleBypass = useCallback((bypassed: boolean) => {
    const filters = filtersRef.current;
    if (bypassed) {
      filters.forEach((f) => { f.gain.value = 0; });
    }
    // When un-bypassing, the caller should re-apply state
  }, []);

  return {
    initAudio,
    connectMic,
    connectFile,
    disconnectSource,
    applyState,
    toggleBypass,
    ctxRef,
    filtersRef,
    analyserRef,
    inputAnalyserRef,
  };
}
