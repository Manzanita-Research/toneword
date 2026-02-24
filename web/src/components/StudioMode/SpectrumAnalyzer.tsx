import { useRef, useEffect, useCallback } from 'react';
import { useAnimationLoop } from '../../hooks/useAnimationLoop';
import { freqToX } from '../../audio/AudioEngine';

interface SpectrumAnalyzerProps {
  analyserRef: React.RefObject<AnalyserNode | null>;
}

const FREQ_LABELS: [number, string][] = [
  [50, '50'], [100, '100'], [200, '200'], [500, '500'],
  [1000, '1k'], [2000, '2k'], [5000, '5k'], [10000, '10k'],
];

function drawFreqGrid(ctx: CanvasRenderingContext2D, w: number, h: number) {
  ctx.font = '9px "Geist Mono", monospace';
  ctx.fillStyle = 'rgba(120, 117, 111, 0.3)';
  ctx.strokeStyle = 'rgba(120, 117, 111, 0.08)';
  ctx.lineWidth = 1;

  FREQ_LABELS.forEach(([freq, label]) => {
    const x = freqToX(freq, w);
    ctx.beginPath();
    ctx.moveTo(x, 0);
    ctx.lineTo(x, h);
    ctx.stroke();
    ctx.fillText(label, x - 8, h - 4);
  });
}

export function SpectrumAnalyzer({ analyserRef }: SpectrumAnalyzerProps) {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const ctxRef = useRef<CanvasRenderingContext2D | null>(null);

  // Handle resize
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const resize = () => {
      const dpr = window.devicePixelRatio || 1;
      const rect = canvas.getBoundingClientRect();
      canvas.width = rect.width * dpr;
      canvas.height = rect.height * dpr;
      const ctx = canvas.getContext('2d');
      if (ctx) {
        ctx.scale(dpr, dpr);
        ctxRef.current = ctx;
      }
    };

    const observer = new ResizeObserver(resize);
    observer.observe(canvas);
    resize();

    return () => observer.disconnect();
  }, []);

  const draw = useCallback(() => {
    const canvas = canvasRef.current;
    const ctx = ctxRef.current;
    const analyser = analyserRef.current;
    if (!canvas || !ctx) return;

    const w = canvas.getBoundingClientRect().width;
    const h = canvas.getBoundingClientRect().height;

    ctx.clearRect(0, 0, w, h);
    drawFreqGrid(ctx, w, h);

    if (!analyser) return;

    const bufferLength = analyser.frequencyBinCount;
    const data = new Uint8Array(bufferLength);
    analyser.getByteFrequencyData(data);

    // Need audio context sample rate for nyquist
    const nyquist = (analyser as any).context?.sampleRate
      ? (analyser as any).context.sampleRate / 2
      : 22050;

    // Filled area
    ctx.beginPath();
    let started = false;
    for (let i = 0; i < bufferLength; i++) {
      const freq = (i / bufferLength) * nyquist;
      if (freq < 20 || freq > 20000) continue;
      const x = freqToX(freq, w);
      const y = h - (data[i] / 255) * (h - 30) - 5;
      if (!started) { ctx.moveTo(x, y); started = true; } else ctx.lineTo(x, y);
    }
    ctx.lineTo(w, h);
    ctx.lineTo(0, h);
    ctx.closePath();

    const gradient = ctx.createLinearGradient(0, 0, 0, h);
    gradient.addColorStop(0, 'rgba(184, 154, 90, 0.4)');
    gradient.addColorStop(1, 'rgba(184, 154, 90, 0.02)');
    ctx.fillStyle = gradient;
    ctx.fill();

    // Stroke line
    ctx.beginPath();
    started = false;
    for (let i = 0; i < bufferLength; i++) {
      const freq = (i / bufferLength) * nyquist;
      if (freq < 20 || freq > 20000) continue;
      const x = freqToX(freq, w);
      const y = h - (data[i] / 255) * (h - 30) - 5;
      if (!started) { ctx.moveTo(x, y); started = true; } else ctx.lineTo(x, y);
    }
    ctx.strokeStyle = 'rgba(184, 154, 90, 0.7)';
    ctx.lineWidth = 1.5;
    ctx.stroke();
  }, [analyserRef]);

  useAnimationLoop(draw);

  return (
    <canvas
      ref={canvasRef}
      className="w-full block"
      style={{ height: '160px' }}
    />
  );
}
