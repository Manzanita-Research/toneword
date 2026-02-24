import { BANDS } from './bands';
import type { SemanticState, Dimension, Band } from './types';

const MAX_GAIN = 12;

/**
 * Apply semantic state to the filter chain.
 * Each band's gain is the sum of weighted contributions from all dimensions,
 * soft-clipped with tanh to prevent extreme values.
 */
export function applyState(
  state: SemanticState,
  filters: BiquadFilterNode[],
  bands: Band[] = BANDS
): void {
  bands.forEach((band, i) => {
    let g = 0;
    for (const dim in band.gainMap) {
      const key = dim as Dimension;
      g += (state[key] / 100) * (band.gainMap[key] ?? 0) * MAX_GAIN;
    }
    g = Math.tanh(g / MAX_GAIN) * MAX_GAIN;
    if (filters[i]) filters[i].gain.value = g;
  });
}

/**
 * Logarithmic frequency-to-x mapping for canvas drawing.
 * Maps 20Hz–20kHz across the given width.
 */
export function freqToX(freq: number, width: number): number {
  return (Math.log10(freq / 20) / Math.log10(1000)) * width;
}

/**
 * Create and chain the 11-band filter array from BANDS config.
 */
export function createFilterChain(ctx: AudioContext): BiquadFilterNode[] {
  const filters = BANDS.map((b) => {
    const f = ctx.createBiquadFilter();
    f.type = b.type;
    f.frequency.value = b.freq;
    f.Q.value = b.baseQ;
    f.gain.value = 0;
    return f;
  });

  for (let i = 0; i < filters.length - 1; i++) {
    filters[i].connect(filters[i + 1]);
  }

  return filters;
}
