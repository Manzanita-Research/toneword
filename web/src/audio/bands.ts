import type { Band } from './types';

/**
 * 11-band EQ configuration — ported verbatim from toneword.html prototype.
 * Every numeric value must match the prototype exactly.
 */
export const BANDS: Band[] = [
  { freq: 60, type: 'lowshelf', baseQ: 0.7, gainMap: { body: 0.6, velvet: 0.3, warmth: 0.2 } },
  { freq: 150, type: 'peaking', baseQ: 1, gainMap: { body: 0.8, warmth: 0.4, velvet: 0.2 } },
  { freq: 300, type: 'peaking', baseQ: 1.2, gainMap: { warmth: 0.9, body: 0.3, velvet: 0.3, glass: -0.3 } },
  { freq: 500, type: 'peaking', baseQ: 1.5, gainMap: { warmth: 0.4, velvet: 0.4, bite: -0.3, glass: -0.4 } },
  { freq: 800, type: 'peaking', baseQ: 1.4, gainMap: { bite: 0.5, glass: 0.4, velvet: -0.5 } },
  { freq: 1500, type: 'peaking', baseQ: 1.2, gainMap: { bite: 0.8, glass: 0.3, warmth: -0.3, velvet: -0.6 } },
  { freq: 2500, type: 'peaking', baseQ: 1, gainMap: { bite: 0.9, glass: 0.6, warmth: -0.5, velvet: -0.8 } },
  { freq: 4000, type: 'peaking', baseQ: 0.9, gainMap: { glass: 0.9, bite: 0.4, air: 0.3, warmth: -0.6, velvet: -0.7 } },
  { freq: 6500, type: 'peaking', baseQ: 0.8, gainMap: { glass: 0.6, air: 0.6, velvet: -0.5, warmth: -0.4 } },
  { freq: 10000, type: 'highshelf', baseQ: 0.7, gainMap: { air: 0.9, glass: 0.3, velvet: -0.6, warmth: -0.5 } },
  { freq: 14000, type: 'peaking', baseQ: 0.6, gainMap: { air: 0.7, glass: 0.2, velvet: -0.3 } },
];
