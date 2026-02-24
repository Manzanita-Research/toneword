import { SliderGroup } from './SliderGroup';
import { PresetBar } from './PresetBar';
import { SpectrumAnalyzer } from './SpectrumAnalyzer';
import { EQCurve } from './EQCurve';
import { ParamTable } from './ParamTable';
import type { Dimension, Preset, SemanticState } from '../../audio/types';

interface StudioModeProps {
  state: SemanticState;
  onDimensionChange: (dim: Dimension, value: number) => void;
  onPresetLoad: (preset: Preset) => void;
  onReset: () => void;
  analyserRef: React.RefObject<AnalyserNode | null>;
  filtersRef: React.RefObject<BiquadFilterNode[]>;
}

export function StudioMode({
  state,
  onDimensionChange,
  onPresetLoad,
  onReset,
  analyserRef,
  filtersRef,
}: StudioModeProps) {
  return (
    <>
      <div className="mb-9 border border-border rounded-lg overflow-hidden bg-surface relative">
        <div className="absolute top-3 left-4 text-[9px] tracking-[0.2em] uppercase text-text-dim z-10">
          Spectrum — real-time output
        </div>
        <SpectrumAnalyzer analyserRef={analyserRef} />
      </div>

      <div className="mb-9 border border-border rounded-lg overflow-hidden bg-surface relative">
        <div className="absolute top-3 left-4 text-[9px] tracking-[0.2em] uppercase text-text-dim z-10">
          EQ Curve — composite response
        </div>
        <EQCurve filtersRef={filtersRef} state={state} />
      </div>

      <SliderGroup state={state} onDimensionChange={onDimensionChange} />
      <PresetBar onPresetLoad={onPresetLoad} onReset={onReset} currentState={state} />
      <ParamTable filtersRef={filtersRef} state={state} />
    </>
  );
}
