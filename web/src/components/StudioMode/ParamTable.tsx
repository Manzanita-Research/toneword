import { useState, useMemo } from 'react';
import { BANDS } from '../../audio/bands';
import { DIMENSIONS } from '../../audio/types';
import type { SemanticState, Dimension } from '../../audio/types';

interface ParamTableProps {
  filtersRef: React.RefObject<BiquadFilterNode[]>;
  state: SemanticState;
}

const MAX_GAIN = 12;

export function ParamTable({ filtersRef: _filtersRef, state }: ParamTableProps) {
  const [open, setOpen] = useState(false);

  const gains = useMemo(() => {
    return BANDS.map((band) => {
      let g = 0;
      for (const dim of DIMENSIONS) {
        const weight = band.gainMap[dim as Dimension];
        if (weight !== undefined) {
          g += (state[dim] / 100) * weight * MAX_GAIN;
        }
      }
      return Math.tanh(g / MAX_GAIN) * MAX_GAIN;
    });
  }, [state]);

  const formatFreq = (freq: number) => {
    return freq >= 1000 ? `${(freq / 1000).toFixed(1)}k` : `${freq}`;
  };

  return (
    <div className="mt-6 px-5 py-4 bg-surface border border-border rounded-lg" style={{ boxShadow: '0 2px 12px rgba(107, 58, 42, 0.06)' }}>
      <button
        className="text-[10px] tracking-[0.15em] uppercase text-text-dim cursor-pointer bg-transparent border-none w-full text-left font-mono hover:text-text-mid transition-colors duration-300"
        onClick={() => setOpen(!open)}
      >
        What's happening under the hood {open ? '\u25BE' : '\u25B8'}
      </button>

      {open && (
        <table className="mt-3 w-full text-[11px] text-text-dim border-collapse font-mono">
          <thead>
            <tr>
              <th className="text-left font-normal text-text-mid py-1 pr-3 border-b border-border">Band</th>
              <th className="text-left font-normal text-text-mid py-1 pr-3 border-b border-border">Type</th>
              <th className="text-left font-normal text-text-mid py-1 pr-3 border-b border-border">Freq</th>
              <th className="text-left font-normal text-text-mid py-1 pr-3 border-b border-border">Gain</th>
              <th className="text-left font-normal text-text-mid py-1 pr-3 border-b border-border">Q</th>
            </tr>
          </thead>
          <tbody>
            {BANDS.map((band, i) => {
              const gain = gains[i];
              const gainColor = gain > 0.1
                ? 'var(--color-body)'
                : gain < -0.1
                ? 'var(--color-air)'
                : 'var(--color-text-dim)';

              return (
                <tr key={i}>
                  <td className="py-1 pr-3 tabular-nums">{i + 1}</td>
                  <td className="py-1 pr-3">{band.type}</td>
                  <td className="py-1 pr-3 tabular-nums">{formatFreq(band.freq)}Hz</td>
                  <td className="py-1 pr-3 tabular-nums" style={{ color: gainColor }}>
                    {gain > 0 ? '+' : ''}{gain.toFixed(1)} dB
                  </td>
                  <td className="py-1 pr-3 tabular-nums">{band.baseQ}</td>
                </tr>
              );
            })}
          </tbody>
        </table>
      )}
    </div>
  );
}
