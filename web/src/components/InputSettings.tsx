import { useState, useEffect, useCallback } from 'react';
import type { MidiConfig } from '../hooks/useMidi';

interface InputSettingsProps {
  open: boolean;
  onClose: () => void;
  selectedDeviceId: string;
  onDeviceChange: (deviceId: string) => void;
  midiConfig: MidiConfig;
  onMidiConfigChange: (update: Partial<MidiConfig>) => void;
  midiAvailable: boolean;
  midiPorts: MIDIInput[];
  lastMidiMessage: string;
}

export function InputSettings({
  open, onClose, selectedDeviceId, onDeviceChange,
  midiConfig, onMidiConfigChange, midiAvailable, midiPorts, lastMidiMessage,
}: InputSettingsProps) {
  const [devices, setDevices] = useState<MediaDeviceInfo[]>([]);
  const [learningCC, setLearningCC] = useState(false);

  const refreshDevices = useCallback(async () => {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
      stream.getTracks().forEach((t) => t.stop());

      const all = await navigator.mediaDevices.enumerateDevices();
      setDevices(all.filter((d) => d.kind === 'audioinput'));
    } catch {
      // Permission denied or no devices
    }
  }, []);

  useEffect(() => {
    if (open) refreshDevices();
  }, [open, refreshDevices]);

  useEffect(() => {
    if (!open) return;
    const handler = () => refreshDevices();
    navigator.mediaDevices.addEventListener('devicechange', handler);
    return () => navigator.mediaDevices.removeEventListener('devicechange', handler);
  }, [open, refreshDevices]);

  // MIDI learn — when in learn mode, capture the next CC
  useEffect(() => {
    if (!learningCC || !midiAvailable) return;

    const handleLearn = (e: Event) => {
      const midi = e as MIDIMessageEvent;
      const data = midi.data;
      if (!data || data.length < 3) return;
      const status = data[0];
      const cc = data[1];
      const isCC = (status & 0xf0) === 0xb0;
      if (!isCC) return;

      const channel = status & 0x0f;
      onMidiConfigChange({ bypassCC: cc, channel });
      setLearningCC(false);
    };

    // Temporarily hijack all MIDI inputs for learning
    const inputs: MIDIInput[] = [];
    if (navigator.requestMIDIAccess) {
      navigator.requestMIDIAccess().then((access) => {
        access.inputs.forEach((input) => {
          inputs.push(input);
          input.addEventListener('midimessage', handleLearn);
        });
      });
    }

    return () => {
      inputs.forEach((input) => {
        input.removeEventListener('midimessage', handleLearn);
      });
    };
  }, [learningCC, midiAvailable, onMidiConfigChange]);

  // Reset learn mode when modal closes
  useEffect(() => {
    if (!open) setLearningCC(false);
  }, [open]);

  if (!open) return null;

  return (
    <>
      <div className="fixed inset-0 bg-black/50 z-[900]" onClick={onClose} />

      <div className="fixed top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 z-[901] w-[380px] max-w-[90vw] bg-surface border border-border rounded-lg p-6 max-h-[85vh] overflow-y-auto" style={{ boxShadow: '0 8px 32px rgba(0,0,0,0.4)' }}>
        <div className="flex items-center justify-between mb-5">
          <h2 className="font-display text-lg tracking-tight">
            Settings
          </h2>
          <button
            className="text-text-dim hover:text-text text-lg leading-none cursor-pointer transition-colors"
            onClick={onClose}
            aria-label="Close"
          >
            &times;
          </button>
        </div>

        {/* Audio Input Section */}
        <section className="mb-6">
          <h3 className="text-[10px] tracking-[0.15em] uppercase text-text-dim mb-3">Audio Input</h3>
          {devices.length === 0 ? (
            <p className="text-text-dim text-xs font-mono">
              No input devices found. Check your browser permissions.
            </p>
          ) : (
            <div className="flex flex-col gap-1.5">
              {devices.map((device) => {
                const active = device.deviceId === selectedDeviceId;
                return (
                  <button
                    key={device.deviceId}
                    className={`text-left font-mono text-xs px-3 py-2.5 rounded-md border cursor-pointer transition-all duration-200 ${
                      active
                        ? 'bg-terracotta/15 border-terracotta text-terracotta'
                        : 'bg-surface-2 border-border text-text hover:border-terracotta/50'
                    }`}
                    onClick={() => {
                      onDeviceChange(device.deviceId);
                    }}
                  >
                    {device.label || `Input ${device.deviceId.slice(0, 8)}...`}
                  </button>
                );
              })}
            </div>
          )}
        </section>

        {/* MIDI Section */}
        <section>
          <h3 className="text-[10px] tracking-[0.15em] uppercase text-text-dim mb-3">MIDI Control</h3>

          {!midiAvailable ? (
            <p className="text-text-dim text-xs font-mono">
              Web MIDI not available in this browser.
            </p>
          ) : (
            <div className="flex flex-col gap-3">
              {/* Enable toggle */}
              <label className="flex items-center gap-2.5 cursor-pointer">
                <input
                  type="checkbox"
                  checked={midiConfig.enabled}
                  onChange={(e) => onMidiConfigChange({ enabled: e.target.checked })}
                  className="accent-terracotta w-3.5 h-3.5 cursor-pointer"
                />
                <span className="font-mono text-xs text-text">Enable MIDI bypass</span>
              </label>

              {midiConfig.enabled && (
                <>
                  {/* Connected devices */}
                  <div className="text-[10px] text-text-dim font-mono">
                    {midiPorts.length === 0
                      ? 'No MIDI devices connected'
                      : `${midiPorts.length} device${midiPorts.length > 1 ? 's' : ''}: ${midiPorts.map((p) => p.name).join(', ')}`
                    }
                  </div>

                  {/* CC number + Learn */}
                  <div className="flex items-center gap-2">
                    <span className="font-mono text-xs text-text-mid">Bypass CC:</span>
                    <input
                      type="number"
                      min={0}
                      max={127}
                      value={midiConfig.bypassCC}
                      onChange={(e) => onMidiConfigChange({ bypassCC: parseInt(e.target.value) || 0 })}
                      className="w-14 font-mono text-xs px-2 py-1.5 bg-surface-2 border border-border rounded text-text text-center outline-none focus:border-terracotta transition-colors"
                    />
                    <button
                      className={`font-mono text-[10px] px-3 py-1.5 rounded-md border cursor-pointer transition-all ${
                        learningCC
                          ? 'bg-terracotta text-bg border-terracotta animate-pulse'
                          : 'bg-surface-2 border-border text-text-mid hover:border-terracotta hover:text-terracotta'
                      }`}
                      onClick={() => setLearningCC(!learningCC)}
                    >
                      {learningCC ? 'Waiting...' : 'Learn'}
                    </button>
                  </div>

                  {/* Channel filter */}
                  <div className="flex items-center gap-2">
                    <span className="font-mono text-xs text-text-mid">Channel:</span>
                    <select
                      value={midiConfig.channel}
                      onChange={(e) => onMidiConfigChange({ channel: parseInt(e.target.value) })}
                      className="font-mono text-xs px-2 py-1.5 bg-surface-2 border border-border rounded text-text outline-none focus:border-terracotta transition-colors cursor-pointer"
                    >
                      <option value={-1}>Any</option>
                      {Array.from({ length: 16 }, (_, i) => (
                        <option key={i} value={i}>{i + 1}</option>
                      ))}
                    </select>
                  </div>

                  {/* Last received message */}
                  {lastMidiMessage && (
                    <div className="text-[10px] text-text-dim font-mono mt-1">
                      Last: {lastMidiMessage}
                    </div>
                  )}
                </>
              )}
            </div>
          )}
        </section>

        <p className="text-[10px] text-text-dim mt-5 leading-relaxed font-mono">
          Send a CC value above 63 to toggle bypass. Use Learn to auto-detect your footswitch CC.
        </p>
      </div>
    </>
  );
}
