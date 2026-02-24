export const DIMENSIONS = ['warmth', 'bite', 'air', 'body', 'glass', 'velvet'] as const;

export type Dimension = (typeof DIMENSIONS)[number];

export type SemanticState = Record<Dimension, number>;

export type BiquadType = 'lowshelf' | 'highshelf' | 'peaking' | 'notch' | 'allpass' | 'lowpass' | 'highpass' | 'bandpass';

export interface Band {
  freq: number;
  type: BiquadType;
  baseQ: number;
  gainMap: Partial<Record<Dimension, number>>;
}

export interface Preset {
  name: string;
  key: string;
  values: SemanticState;
}

export const DIMENSION_COLORS: Record<Dimension, string> = {
  warmth: '#c4723a',
  bite: '#b85a5a',
  air: '#5a8ab8',
  body: '#b89a5a',
  glass: '#5ab8a8',
  velvet: '#8a5ab8',
};

export const DIMENSION_DESCRIPTIONS: Record<Dimension, string> = {
  warmth: 'Low-mid presence, gentle high rolloff. Tube amp glow.',
  bite: 'Upper-mid aggression. Pick attack. Cuts through a mix.',
  air: 'Shimmer and openness above 8kHz. Acoustic sparkle.',
  body: 'Fundamental weight. The chest-resonance of a dreadnought.',
  glass: 'Crystalline presence. Fender cleans. Bell-like clarity.',
  velvet: 'Smooth darkness. Jazz neck pickup. Rolled-off silk.',
};

export function createDefaultState(): SemanticState {
  return { warmth: 0, bite: 0, air: 0, body: 0, glass: 0, velvet: 0 };
}
