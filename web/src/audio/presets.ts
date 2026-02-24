import type { Preset } from './types';

/**
 * 8 factory tone recipe presets — ported verbatim from toneword.html prototype.
 */
export const PRESETS: Preset[] = [
  {
    name: 'Flat',
    key: 'flat',
    values: { warmth: 0, bite: 0, air: 0, body: 0, glass: 0, velvet: 0 },
  },
  {
    name: 'Nashville Twang',
    key: 'nashville',
    values: { warmth: -20, bite: 60, air: 40, body: -10, glass: 50, velvet: -40 },
  },
  {
    name: 'Hendrix Clean',
    key: 'hendrix',
    values: { warmth: 40, bite: 20, air: 10, body: 30, glass: 30, velvet: -10 },
  },
  {
    name: 'Midnight Jazz',
    key: 'jazz',
    values: { warmth: 50, bite: -60, air: -30, body: 40, glass: -40, velvet: 80 },
  },
  {
    name: 'Punk Scoop',
    key: 'punk',
    values: { warmth: -30, bite: 80, air: -10, body: 50, glass: -20, velvet: -60 },
  },
  {
    name: 'Lo-fi Tape',
    key: 'lofi',
    values: { warmth: 60, bite: -40, air: -60, body: 20, glass: -50, velvet: 50 },
  },
  {
    name: 'Open Acoustic',
    key: 'acoustic',
    values: { warmth: 20, bite: 10, air: 60, body: 50, glass: 40, velvet: -20 },
  },
  {
    name: 'Scooped Metal',
    key: 'metal',
    values: { warmth: -20, bite: 70, air: 20, body: 70, glass: -30, velvet: -70 },
  },
];
