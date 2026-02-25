import { useState, useCallback, useMemo } from 'react';
import { Knob } from './Knob';
import { OledDisplay } from './OledDisplay';
import { Footswitch } from './Footswitch';
import { PresetNav } from './PresetNav';
import { DIMENSIONS, DIMENSION_COLORS } from '../../audio/types';
import { PRESETS } from '../../audio/presets';
import type { Dimension, Preset, SemanticState } from '../../audio/types';

interface PedalModeProps {
  state: SemanticState;
  onDimensionChange: (dim: Dimension, value: number) => void;
  onPresetLoad: (preset: Preset) => void;
  bypassed: boolean;
  onBypassToggle: () => void;
  analyserRef: React.RefObject<AnalyserNode | null>;
  filtersRef: React.RefObject<BiquadFilterNode[]>;
}

// Top row: warmth, bite, air (large knobs)
// Bottom row: body, glass, velvet (small knobs)
const TOP_ROW: Dimension[] = ['warmth', 'bite', 'air'];
const BOTTOM_ROW: Dimension[] = ['body', 'glass', 'velvet'];

export function PedalMode({
  state,
  onDimensionChange,
  onPresetLoad,
  bypassed,
  onBypassToggle,
  analyserRef,
  filtersRef,
}: PedalModeProps) {
  const [presetIndex, setPresetIndex] = useState(-1);

  // Detect current preset name
  const presetName = useMemo(() => {
    for (let i = 0; i < PRESETS.length; i++) {
      const p = PRESETS[i];
      const matches = DIMENSIONS.every((dim) => Math.abs(state[dim] - p.values[dim]) <= 3);
      if (matches) return p.name;
    }
    return '\u2014 custom \u2014';
  }, [state]);

  const handlePresetPrev = useCallback(() => {
    let idx = presetIndex - 1;
    if (idx < 0) idx = PRESETS.length - 1;
    setPresetIndex(idx);
    onPresetLoad(PRESETS[idx]);
  }, [presetIndex, onPresetLoad]);

  const handlePresetNext = useCallback(() => {
    let idx = presetIndex + 1;
    if (idx >= PRESETS.length) idx = 0;
    setPresetIndex(idx);
    onPresetLoad(PRESETS[idx]);
  }, [presetIndex, onPresetLoad]);

  return (
    <div className="flex flex-col items-center justify-center" style={{ perspective: '900px' }}>
      {/* Pedal enclosure */}
      <div
        className="w-[340px] h-[640px] rounded-2xl relative flex flex-col items-center overflow-hidden"
        style={{
          background: 'linear-gradient(175deg, #252320 0%, #1a1816 40%, #141210 100%)',
          boxShadow: '0 2px 0 0 #3a3835, 0 4px 0 0 #2a2827, 0 20px 60px rgba(107, 58, 42, 0.15), inset 0 1px 0 rgba(255,255,255,0.06)',
          transform: 'rotateX(4deg)',
          padding: '24px 24px 28px',
        }}
      >
        {/* Brushed metal texture */}
        <div
          className="absolute inset-0 rounded-2xl pointer-events-none"
          style={{
            background: 'repeating-linear-gradient(90deg, transparent, transparent 1px, rgba(255,255,255,0.008) 1px, rgba(255,255,255,0.008) 2px)',
          }}
        />

        {/* Screws */}
        {['top-2.5 left-2.5', 'top-2.5 right-2.5', 'bottom-2.5 left-2.5', 'bottom-2.5 right-2.5'].map((pos, i) => (
          <div
            key={i}
            className={`absolute ${pos} w-2.5 h-2.5 rounded-full`}
            style={{
              background: 'radial-gradient(circle at 35% 35%, #3a3835, #1a1816)',
              boxShadow: 'inset 0 1px 2px rgba(0,0,0,0.6), 0 0.5px 0 rgba(255,255,255,0.08)',
            }}
          >
            <div
              className="absolute"
              style={{
                top: '50%',
                left: '50%',
                width: '6px',
                height: '1px',
                background: '#141210',
                transform: 'translate(-50%, -50%) rotate(30deg)',
              }}
            />
          </div>
        ))}

        {/* Brand */}
        <div
          className="font-display text-xl tracking-[-0.01em] text-white/50 mb-0.5 text-center z-[2]"
        >
          TONE<em className="text-body/70 italic">WORD</em>
        </div>
        <div className="font-mono text-[9px] tracking-[0.2em] uppercase text-text-dim/40 mb-4 z-[2]">
          Semantic EQ
        </div>

        {/* OLED Display */}
        <OledDisplay
          state={state}
          presetName={presetName}
          bypassed={bypassed}
          analyserRef={analyserRef}
          filtersRef={filtersRef}
        />

        {/* Top row knobs (large) */}
        <div className="flex gap-5 mb-3 z-[2] w-full justify-center">
          {TOP_ROW.map((dim) => (
            <Knob
              key={dim}
              value={state[dim]}
              color={DIMENSION_COLORS[dim]}
              label={dim.charAt(0).toUpperCase() + dim.slice(1)}
              onChange={(v) => onDimensionChange(dim, v)}
            />
          ))}
        </div>

        {/* Bottom row knobs (small) */}
        <div className="flex gap-5 mb-3 z-[2] w-full justify-center">
          {BOTTOM_ROW.map((dim) => (
            <Knob
              key={dim}
              value={state[dim]}
              color={DIMENSION_COLORS[dim]}
              label={dim.charAt(0).toUpperCase() + dim.slice(1)}
              onChange={(v) => onDimensionChange(dim, v)}
              small
            />
          ))}
        </div>

        {/* Preset nav */}
        <PresetNav
          currentPresetName={presetName}
          onPrev={handlePresetPrev}
          onNext={handlePresetNext}
        />

        {/* Footswitch */}
        <Footswitch bypassed={bypassed} onToggle={onBypassToggle} />
      </div>

      {/* Jacks */}
      <div className="flex justify-between w-[340px] px-[36px] mt-1.5">
        <span className="text-[7px] tracking-[0.1em] uppercase text-text-dim/30">&larr; Input</span>
        <span className="text-[7px] tracking-[0.1em] uppercase text-text-dim/30">Output &rarr;</span>
      </div>
    </div>
  );
}
