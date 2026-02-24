import { useState, useEffect, useCallback } from 'react';
import { useAudioEngine } from './hooks/useAudioEngine';
import { useSemanticState } from './hooks/useSemanticState';
import { ModeToggle } from './components/ModeToggle';
import { SourceBar } from './components/SourceBar';
import { StudioMode } from './components/StudioMode';
import { PedalMode } from './components/PedalMode';
import type { Dimension, Preset } from './audio/types';

function App() {
  const [mode, setMode] = useState<'studio' | 'pedal'>('studio');
  const [source, setSource] = useState('');
  const [bypassed, setBypassed] = useState(false);

  const engine = useAudioEngine();
  const { state, setDimension, loadPreset, resetAll } = useSemanticState();

  // Apply state to audio engine whenever semantic state changes
  useEffect(() => {
    if (!bypassed) {
      engine.applyState(state);
    }
  }, [state, bypassed, engine]);

  const handleMic = useCallback(async () => {
    try {
      await engine.connectMic();
      setSource('mic');
    } catch {
      alert('Microphone access needed.');
    }
  }, [engine]);

  const handleFile = useCallback(async (file: File) => {
    try {
      const name = await engine.connectFile(file);
      setSource(name);
    } catch (err) {
      console.error('Failed to load audio file:', err);
    }
  }, [engine]);

  const handleBypassToggle = useCallback(() => {
    const next = !bypassed;
    setBypassed(next);
    engine.toggleBypass(next);
    if (!next) {
      engine.applyState(state);
    }
  }, [bypassed, engine, state]);

  const handleDimensionChange = useCallback((dim: Dimension, value: number) => {
    setDimension(dim, value);
  }, [setDimension]);

  const handlePresetLoad = useCallback((preset: Preset) => {
    loadPreset(preset);
  }, [loadPreset]);

  const handleReset = useCallback(() => {
    resetAll();
  }, [resetAll]);

  return (
    <>
      <ModeToggle mode={mode} onModeChange={setMode} />

      {mode === 'studio' ? (
        <div className="max-w-[900px] mx-auto pt-[72px] px-6 pb-10">
          <header className="mb-12">
            <div
              className="font-display text-[42px] tracking-[-0.02em] leading-[1.1] mb-1.5"
              style={{ fontVariationSettings: "'WONK' 1, 'opsz' 42" }}
            >
              TONE<span className="italic text-body">WORD</span>
            </div>
            <div className="font-body text-[13px] tracking-[0.02em] text-text-dim" style={{ fontVariationSettings: "'opsz' 14" }}>
              Semantic EQ — speak your tone into being
            </div>
          </header>

          <SourceBar
            onMic={handleMic}
            onFile={handleFile}
            source={source}
            bypassed={bypassed}
            onBypassToggle={handleBypassToggle}
            inputAnalyserRef={engine.inputAnalyserRef}
          />

          <StudioMode
            state={state}
            onDimensionChange={handleDimensionChange}
            onPresetLoad={handlePresetLoad}
            onReset={handleReset}
            analyserRef={engine.analyserRef}
            filtersRef={engine.filtersRef}
          />
        </div>
      ) : (
        <div className="flex flex-col items-center justify-center min-h-screen pt-[72px] px-6 pb-10">
          <PedalMode
            state={state}
            onDimensionChange={handleDimensionChange}
            onPresetLoad={handlePresetLoad}
            bypassed={bypassed}
            onBypassToggle={handleBypassToggle}
            analyserRef={engine.analyserRef}
            filtersRef={engine.filtersRef}
          />

          <div className="flex gap-2 mt-5 items-center">
            <button
              className="font-mono text-[10px] px-3 py-1.5 bg-surface-2 border border-border text-text rounded-md cursor-pointer hover:border-terracotta hover:text-terracotta transition-all"
              onClick={handleMic}
            >
              Guitar In
            </button>
            <button
              className="font-mono text-[10px] px-3 py-1.5 bg-surface-2 border border-border text-text rounded-md cursor-pointer hover:border-terracotta hover:text-terracotta transition-all"
              onClick={() => {
                const input = document.createElement('input');
                input.type = 'file';
                input.accept = 'audio/*';
                input.onchange = (e) => {
                  const file = (e.target as HTMLInputElement).files?.[0];
                  if (file) handleFile(file);
                };
                input.click();
              }}
            >
              Load Audio
            </button>
          </div>

          <div className="mt-4 text-[10px] text-text-dim text-center max-w-[300px] leading-relaxed">
            Drag knobs up/down to adjust. Click footswitch to bypass.<br />
            Use &#9666; &#9656; to browse presets.
          </div>
        </div>
      )}
    </>
  );
}

export default App;
