import { useState, useEffect, useCallback } from 'react';
import { useAudioEngine } from './hooks/useAudioEngine';
import { useSemanticState } from './hooks/useSemanticState';
import { useMidi } from './hooks/useMidi';
import { ModeToggle } from './components/ModeToggle';
import { SourceBar } from './components/SourceBar';
import { InputSettings } from './components/InputSettings';
import { StudioMode } from './components/StudioMode';
import { PedalMode } from './components/PedalMode';
import type { Dimension, Preset } from './audio/types';

function App() {
  const [mode, setMode] = useState<'studio' | 'pedal'>('studio');
  const [source, setSource] = useState('');
  const [bypassed, setBypassed] = useState(false);
  const [settingsOpen, setSettingsOpen] = useState(false);
  const [selectedDeviceId, setSelectedDeviceId] = useState(() =>
    localStorage.getItem('toneword:inputDeviceId') || ''
  );
  const [pendingConnect, setPendingConnect] = useState(false);

  const engine = useAudioEngine();
  const { state, setDimension, loadPreset, resetAll } = useSemanticState();

  const handleBypassToggle = useCallback(() => {
    setBypassed((prev) => {
      const next = !prev;
      engine.toggleBypass(next);
      if (!next) {
        engine.applyState(state);
      }
      return next;
    });
  }, [engine, state]);

  const midi = useMidi(handleBypassToggle);

  // Apply state to audio engine whenever semantic state changes
  useEffect(() => {
    if (!bypassed) {
      engine.applyState(state);
    }
  }, [state, bypassed, engine]);

  const handleDeviceChange = useCallback((deviceId: string) => {
    setSelectedDeviceId(deviceId);
    localStorage.setItem('toneword:inputDeviceId', deviceId);
    if (source === 'mic') {
      // Already active — reconnect with the new device
      engine.connectMic(deviceId).catch(() => {});
    } else if (pendingConnect) {
      // User clicked Guitar In but had no device — now they picked one, auto-connect
      engine.connectMic(deviceId).then(() => {
        setSource('mic');
      }).catch(() => {});
      setPendingConnect(false);
    }
  }, [engine, source, pendingConnect]);

  const handleMic = useCallback(async () => {
    if (source === 'mic') {
      engine.disconnectSource();
      setSource('');
      setPendingConnect(false);
      return;
    }
    // No device selected yet — open settings so user picks one first
    if (!selectedDeviceId) {
      setPendingConnect(true);
      setSettingsOpen(true);
      return;
    }
    try {
      await engine.connectMic(selectedDeviceId);
      setSource('mic');
      setSettingsOpen(true);
    } catch {
      // Permission denied or device unavailable — open settings to retry
      setSettingsOpen(true);
    }
  }, [engine, selectedDeviceId, source]);

  const handleFile = useCallback(async (file: File) => {
    try {
      const name = await engine.connectFile(file);
      setSource(name);
    } catch (err) {
      console.error('Failed to load audio file:', err);
    }
  }, [engine]);

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
            >
              TONE<span className="italic text-body">WORD</span>
            </div>
            <div className="font-mono text-[13px] tracking-[0.02em] text-text-dim">
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
            onSettingsOpen={() => setSettingsOpen(true)}
            hasDeviceSelected={!!selectedDeviceId}
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
            <div className="flex items-center">
              <button
                className="font-mono text-[10px] px-3 py-1.5 bg-surface-2 border border-r-0 border-border text-text rounded-l-md cursor-pointer hover:border-terracotta hover:text-terracotta transition-all"
                onClick={handleMic}
              >
                Guitar In
              </button>
              <button
                className={`font-mono text-[10px] px-1.5 py-1.5 bg-surface-2 border rounded-r-md cursor-pointer transition-all ${
                  selectedDeviceId
                    ? 'border-terracotta/50 text-terracotta hover:bg-terracotta/10'
                    : 'border-border text-text-dim hover:border-terracotta hover:text-terracotta'
                }`}
                onClick={() => setSettingsOpen(true)}
                aria-label="Audio input settings"
                title="Choose input device"
              >
                <svg width="12" height="12" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                  <path d="M12.22 2h-.44a2 2 0 0 0-2 2v.18a2 2 0 0 1-1 1.73l-.43.25a2 2 0 0 1-2 0l-.15-.08a2 2 0 0 0-2.73.73l-.22.38a2 2 0 0 0 .73 2.73l.15.1a2 2 0 0 1 1 1.72v.51a2 2 0 0 1-1 1.74l-.15.09a2 2 0 0 0-.73 2.73l.22.38a2 2 0 0 0 2.73.73l.15-.08a2 2 0 0 1 2 0l.43.25a2 2 0 0 1 1 1.73V20a2 2 0 0 0 2 2h.44a2 2 0 0 0 2-2v-.18a2 2 0 0 1 1-1.73l.43-.25a2 2 0 0 1 2 0l.15.08a2 2 0 0 0 2.73-.73l.22-.39a2 2 0 0 0-.73-2.73l-.15-.08a2 2 0 0 1-1-1.74v-.5a2 2 0 0 1 1-1.74l.15-.09a2 2 0 0 0 .73-2.73l-.22-.38a2 2 0 0 0-2.73-.73l-.15.08a2 2 0 0 1-2 0l-.43-.25a2 2 0 0 1-1-1.73V4a2 2 0 0 0-2-2z" />
                  <circle cx="12" cy="12" r="3" />
                </svg>
              </button>
            </div>
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

      <InputSettings
        open={settingsOpen}
        onClose={() => setSettingsOpen(false)}
        selectedDeviceId={selectedDeviceId}
        onDeviceChange={handleDeviceChange}
        midiConfig={midi.config}
        onMidiConfigChange={midi.updateConfig}
        midiAvailable={midi.midiAvailable}
        midiPorts={midi.midiPorts}
        lastMidiMessage={midi.lastMessage}
      />
    </>
  );
}

export default App;
