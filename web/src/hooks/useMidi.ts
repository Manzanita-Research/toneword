import { useEffect, useRef, useCallback, useState } from 'react';

export interface MidiConfig {
  enabled: boolean;
  bypassCC: number; // CC number to listen for
  channel: number; // 0-15, or -1 for any channel
}

const STORAGE_KEY = 'toneword:midiConfig';

const DEFAULT_CONFIG: MidiConfig = {
  enabled: true,
  bypassCC: 80,
  channel: -1, // any channel
};

function loadConfig(): MidiConfig {
  try {
    const stored = localStorage.getItem(STORAGE_KEY);
    if (stored) return { ...DEFAULT_CONFIG, ...JSON.parse(stored) };
  } catch { /* use defaults */ }
  return DEFAULT_CONFIG;
}

function saveConfig(config: MidiConfig) {
  localStorage.setItem(STORAGE_KEY, JSON.stringify(config));
}

export function useMidi(onBypassToggle: () => void) {
  const [config, setConfig] = useState<MidiConfig>(loadConfig);
  const [midiAccess, setMidiAccess] = useState<MIDIAccess | null>(null);
  const [midiPorts, setMidiPorts] = useState<MIDIInput[]>([]);
  const [lastMessage, setLastMessage] = useState('');

  const configRef = useRef(config);
  configRef.current = config;

  const onBypassRef = useRef(onBypassToggle);
  onBypassRef.current = onBypassToggle;

  // Request MIDI access
  useEffect(() => {
    if (!navigator.requestMIDIAccess) return;

    navigator.requestMIDIAccess().then((access) => {
      setMidiAccess(access);
      const inputs = Array.from(access.inputs.values());
      setMidiPorts(inputs);

      access.onstatechange = () => {
        setMidiPorts(Array.from(access.inputs.values()));
      };
    }).catch(() => {
      // MIDI not available
    });
  }, []);

  // Handle incoming MIDI messages
  const handleMidiMessage = useCallback((e: MIDIMessageEvent) => {
    const data = e.data;
    if (!data || data.length < 3) return;

    const status = data[0];
    const cc = data[1];
    const value = data[2];

    // CC messages are 0xB0-0xBF (176-191)
    const isCC = (status & 0xf0) === 0xb0;
    if (!isCC) return;

    const channel = status & 0x0f;
    const cfg = configRef.current;

    if (!cfg.enabled) return;
    if (cfg.channel !== -1 && cfg.channel !== channel) return;

    if (cc === cfg.bypassCC) {
      setLastMessage(`CC${cc} ch${channel + 1} val=${value}`);
      // Toggle bypass on every CC hit — works with both momentary (always 127)
      // and toggle (alternating 127/0) footswitch modes
      onBypassRef.current();
    }
  }, []);

  // Attach listeners to all MIDI inputs
  useEffect(() => {
    if (!midiAccess) return;

    const inputs = Array.from(midiAccess.inputs.values());
    inputs.forEach((input) => {
      input.onmidimessage = handleMidiMessage;
    });

    return () => {
      inputs.forEach((input) => {
        input.onmidimessage = null;
      });
    };
  }, [midiAccess, midiPorts, handleMidiMessage]);

  const updateConfig = useCallback((update: Partial<MidiConfig>) => {
    setConfig((prev) => {
      const next = { ...prev, ...update };
      saveConfig(next);
      return next;
    });
  }, []);

  return {
    config,
    updateConfig,
    midiAvailable: !!midiAccess,
    midiPorts,
    lastMessage,
  };
}
