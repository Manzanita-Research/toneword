import { PRESETS } from '../../audio/presets';
import { DIMENSIONS } from '../../audio/types';
import type { Preset, SemanticState } from '../../audio/types';

interface PresetBarProps {
  onPresetLoad: (preset: Preset) => void;
  onReset: () => void;
  currentState: SemanticState;
}

function isPresetActive(preset: Preset, state: SemanticState): boolean {
  return DIMENSIONS.every((dim) => Math.abs(state[dim] - preset.values[dim]) <= 3);
}

export function PresetBar({ onPresetLoad, onReset, currentState }: PresetBarProps) {
  return (
    <div className="mb-9">
      <div className="text-[10px] tracking-[0.15em] uppercase text-text-dim mb-5 pb-2 border-b border-border font-mono">
        Tone Recipes
      </div>
      <div className="flex flex-wrap gap-2 mt-4">
        {PRESETS.map((preset) => {
          const active = isPresetActive(preset, currentState);
          return (
            <button
              key={preset.key}
              className={`font-mono text-[11px] px-3.5 py-1.5 border rounded-full cursor-pointer transition-all duration-300 ease-out ${
                active
                  ? 'bg-terracotta text-bg border-terracotta'
                  : 'bg-transparent text-text-mid border-border hover:border-terracotta hover:text-terracotta'
              }`}
              style={active ? { boxShadow: '0 2px 10px rgba(194, 113, 79, 0.25)' } : undefined}
              onClick={() => onPresetLoad(preset)}
            >
              {preset.name}
            </button>
          );
        })}
        <button
          className="font-mono text-[11px] px-3.5 py-1.5 border rounded-full cursor-pointer transition-all duration-300 ease-out bg-transparent text-text-dim border-border hover:border-bite hover:text-bite"
          onClick={onReset}
        >
          Reset
        </button>
      </div>
    </div>
  );
}
