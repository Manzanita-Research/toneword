import { useState, useCallback } from 'react';
import type { Dimension, SemanticState, Preset } from '../audio/types';
import { createDefaultState } from '../audio/types';

export function useSemanticState() {
  const [state, setState] = useState<SemanticState>(createDefaultState);

  const setDimension = useCallback((dim: Dimension, value: number) => {
    setState((prev) => ({ ...prev, [dim]: value }));
  }, []);

  const loadPreset = useCallback((preset: Preset) => {
    setState({ ...preset.values });
  }, []);

  const resetAll = useCallback(() => {
    setState(createDefaultState());
  }, []);

  return { state, setDimension, loadPreset, resetAll };
}
