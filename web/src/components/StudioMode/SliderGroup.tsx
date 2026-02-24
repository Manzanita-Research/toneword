import { DIMENSIONS, DIMENSION_COLORS, DIMENSION_DESCRIPTIONS } from '../../audio/types';
import type { Dimension, SemanticState } from '../../audio/types';

interface SliderGroupProps {
  state: SemanticState;
  onDimensionChange: (dim: Dimension, value: number) => void;
}

export function SliderGroup({ state, onDimensionChange }: SliderGroupProps) {
  return (
    <div className="mb-9">
      <div className="text-[10px] tracking-[0.15em] uppercase text-text-dim mb-5 pb-2 border-b border-border font-mono">
        Tone Words
      </div>
      <div className="grid grid-cols-1 sm:grid-cols-2 gap-5 gap-x-8">
        {DIMENSIONS.map((dim) => (
          <div key={dim} className="group">
            <div className="flex justify-between items-baseline mb-2.5">
              <span
                className="font-display text-xl tracking-[-0.01em] transition-colors duration-300"
                style={{
                  color: DIMENSION_COLORS[dim],
                  fontVariationSettings: "'WONK' 1, 'opsz' 20",
                }}
              >
                {dim.charAt(0).toUpperCase() + dim.slice(1)}
              </span>
              <span className="font-mono text-[11px] text-text-dim tabular-nums">
                {state[dim] > 0 ? '+' : ''}{state[dim]}
              </span>
            </div>
            <div className="font-body text-[11px] text-text-dim mb-2.5 leading-relaxed" style={{ fontVariationSettings: "'opsz' 11" }}>
              {DIMENSION_DESCRIPTIONS[dim]}
            </div>
            <input
              type="range"
              className="semantic-slider"
              style={{ '--thumb-color': DIMENSION_COLORS[dim] } as React.CSSProperties}
              min={-100}
              max={100}
              step={1}
              value={state[dim]}
              onChange={(e) => onDimensionChange(dim, Number(e.target.value))}
              onDoubleClick={() => onDimensionChange(dim, 0)}
            />
          </div>
        ))}
      </div>
    </div>
  );
}
