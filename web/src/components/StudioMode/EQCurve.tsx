import { useRef, useEffect, useCallback } from 'react';
import { useAnimationLoop } from '../../hooks/useAnimationLoop';
import { freqToX } from '../../audio/AudioEngine';
import type { SemanticState } from '../../audio/types';

interface EQCurveProps {
  filtersRef: React.RefObject<BiquadFilterNode[]>;
  state: SemanticState;
}

const FREQ_LABELS: [number, string][] = [
  [50, '50'], [100, '100'], [200, '200'], [500, '500'],
  [1000, '1k'], [2000, '2k'], [5000, '5k'], [10000, '10k'],
];

function drawFreqGrid(ctx: CanvasRenderingContext2D, w: number, h: number) {
  ctx.font = '9px "Geist Mono", monospace';
  ctx.fillStyle = 'rgba(120, 117, 111, 0.3)';
  ctx.strokeStyle = 'rgba(120, 117, 111, 0.08)';
  ctx.lineWidth = 1;

  FREQ_LABELS.forEach(([freq, label]) => {
    const x = freqToX(freq, w);
    ctx.beginPath();
    ctx.moveTo(x, 0);
    ctx.lineTo(x, h);
    ctx.stroke();
    ctx.fillText(label, x - 8, h - 4);
  });
}

const NUM_POINTS = 200;

export function EQCurve({ filtersRef, state: _state }: EQCurveProps) {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const ctxRef = useRef<CanvasRenderingContext2D | null>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const resize = () => {
      const dpr = window.devicePixelRatio || 1;
      const rect = canvas.getBoundingClientRect();
      canvas.width = rect.width * dpr;
      canvas.height = rect.height * dpr;
      const ctx = canvas.getContext('2d');
      if (ctx) {
        ctx.scale(dpr, dpr);
        ctxRef.current = ctx;
      }
    };

    const observer = new ResizeObserver(resize);
    observer.observe(canvas);
    resize();

    return () => observer.disconnect();
  }, []);

  const draw = useCallback(() => {
    const canvas = canvasRef.current;
    const ctx = ctxRef.current;
    const filters = filtersRef.current;
    if (!canvas || !ctx) return;

    const w = canvas.getBoundingClientRect().width;
    const h = canvas.getBoundingClientRect().height;

    ctx.clearRect(0, 0, w, h);
    drawFreqGrid(ctx, w, h);

    if (!filters.length) return;

    // Generate logarithmically spaced frequencies
    const freqs = new Float32Array(NUM_POINTS);
    const composite = new Float32Array(NUM_POINTS);
    for (let i = 0; i < NUM_POINTS; i++) {
      freqs[i] = 20 * Math.pow(1000, i / (NUM_POINTS - 1));
      composite[i] = 0;
    }

    // Sum frequency responses from all filters
    filters.forEach((f) => {
      const magResponse = new Float32Array(NUM_POINTS);
      const phaseResponse = new Float32Array(NUM_POINTS);
      f.getFrequencyResponse(freqs, magResponse, phaseResponse);
      for (let i = 0; i < NUM_POINTS; i++) {
        composite[i] += 20 * Math.log10(magResponse[i]);
      }
    });

    const midY = h / 2;
    const dBRange = 24;

    // Filled area
    ctx.beginPath();
    for (let i = 0; i < NUM_POINTS; i++) {
      const x = freqToX(freqs[i], w);
      const y = midY - (composite[i] / dBRange) * (h - 40);
      if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
    }
    ctx.lineTo(freqToX(freqs[NUM_POINTS - 1], w), midY);
    ctx.lineTo(freqToX(freqs[0], w), midY);
    ctx.closePath();

    const gradient = ctx.createLinearGradient(0, 0, 0, h);
    gradient.addColorStop(0, 'rgba(196, 114, 58, 0.25)');
    gradient.addColorStop(0.5, 'rgba(196, 114, 58, 0.05)');
    gradient.addColorStop(1, 'rgba(90, 138, 184, 0.25)');
    ctx.fillStyle = gradient;
    ctx.fill();

    // Stroke line
    ctx.beginPath();
    for (let i = 0; i < NUM_POINTS; i++) {
      const x = freqToX(freqs[i], w);
      const y = midY - (composite[i] / dBRange) * (h - 40);
      if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
    }
    ctx.strokeStyle = 'rgba(232, 228, 223, 0.8)';
    ctx.lineWidth = 2;
    ctx.stroke();

    // 0dB center line
    ctx.beginPath();
    ctx.moveTo(0, midY);
    ctx.lineTo(w, midY);
    ctx.strokeStyle = 'rgba(120, 117, 111, 0.3)';
    ctx.lineWidth = 1;
    ctx.setLineDash([4, 4]);
    ctx.stroke();
    ctx.setLineDash([]);

    // dB labels
    ctx.font = '9px "Geist Mono", monospace';
    ctx.fillStyle = 'rgba(120, 117, 111, 0.5)';
    ctx.fillText('+12dB', 4, midY - (h - 40) / 2 + 4);
    ctx.fillText('-12dB', 4, midY + (h - 40) / 2 + 4);
    ctx.fillText('0dB', 4, midY + 3);
  }, [filtersRef, _state]);

  useAnimationLoop(draw);

  return (
    <canvas
      ref={canvasRef}
      className="w-full block"
      style={{ height: '140px' }}
    />
  );
}
