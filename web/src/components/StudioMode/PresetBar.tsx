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
      <div className="text-[10px] tracking-[0.2em] uppercase text-text-dim mb-5 pb-2 border-b border-border">
        Tone Recipes
      </div>
      <div className="flex flex-wrap gap-2 mt-4">
        {PRESETS.map((preset) => {
          const active = isPresetActive(preset, currentState);
          return (
            <button
              key={preset.key}
              className={`font-mono text-[11px] px-3.5 py-1.5 border rounded-full cursor-pointer transition-all duration-200 ${
                active
                  ? 'bg-body text-bg border-body'
                  : 'bg-transparent text-text-mid border-border hover:border-body hover:text-body'
              }`}
              onClick={() => onPresetLoad(preset)}
            >
              {preset.name}
            </button>
          );
        })}
        <button
          className="font-mono text-[11px] px-3.5 py-1.5 border rounded-full cursor-pointer transition-all duration-200 bg-transparent text-text-dim border-border hover:border-bite hover:text-bite"
          onClick={onReset}
        >
          Reset
        </button>
      </div>
    </div>
  );
}
