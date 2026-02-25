import { useRef, useEffect, useCallback } from 'react';
import { useAnimationLoop } from '../../hooks/useAnimationLoop';
import { DIMENSIONS, DIMENSION_COLORS } from '../../audio/types';
import type { SemanticState } from '../../audio/types';

interface OledDisplayProps {
  state: SemanticState;
  presetName: string;
  bypassed: boolean;
  analyserRef: React.RefObject<AnalyserNode | null>;
  filtersRef: React.RefObject<BiquadFilterNode[]>;
}

export function OledDisplay({ state, presetName, bypassed, analyserRef, filtersRef }: OledDisplayProps) {
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
    if (!canvas || !ctx) return;

    const w = canvas.getBoundingClientRect().width;
    const h = canvas.getBoundingClientRect().height;

    ctx.clearRect(0, 0, w, h);
    ctx.fillStyle = '#000';
    ctx.fillRect(0, 0, w, h);

    const analyser = analyserRef.current;
    const filters = filtersRef.current;

    // Mini spectrum (if audio active and not bypassed)
    if (analyser && !bypassed) {
      const bufferLength = analyser.frequencyBinCount;
      const data = new Uint8Array(bufferLength);
      analyser.getByteFrequencyData(data);
      const nyquist = (analyser as any).context?.sampleRate
        ? (analyser as any).context.sampleRate / 2
        : 22050;

      const px = 8, cw = w - 16, ct = 8, ch = 56;
      ctx.beginPath();
      let started = false;
      for (let i = 0; i < bufferLength; i++) {
        const freq = (i / bufferLength) * nyquist;
        if (freq < 20 || freq > 20000) continue;
        const x = px + (Math.log10(freq / 20) / Math.log10(1000)) * cw;
        const magnitude = data[i] / 255;
        const y = ct + ch - magnitude * ch * 0.6;
        if (!started) { ctx.moveTo(x, y); started = true; } else ctx.lineTo(x, y);
      }
      ctx.lineTo(px + cw, ct + ch);
      ctx.lineTo(px, ct + ch);
      ctx.closePath();
      ctx.fillStyle = 'rgba(184, 154, 90, 0.08)';
      ctx.fill();
    }

    // EQ curve
    if (filters.length) {
      const np = 120;
      const fqs = new Float32Array(np);
      const resp = new Float32Array(np);
      for (let i = 0; i < np; i++) {
        fqs[i] = 20 * Math.pow(1000, i / (np - 1));
        resp[i] = 0;
      }
      filters.forEach((f) => {
        const mg = new Float32Array(np);
        const ph = new Float32Array(np);
        f.getFrequencyResponse(fqs, mg, ph);
        for (let i = 0; i < np; i++) resp[i] += 20 * Math.log10(mg[i]);
      });

      const ct = 8, ch = 56, midY = ct + ch / 2, dBRange = 24, px = 8, cw = w - 16;

      // Fill
      ctx.beginPath();
      for (let i = 0; i < np; i++) {
        const x = px + (i / (np - 1)) * cw;
        const y = midY - (resp[i] / dBRange) * ch;
        if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
      }
      ctx.lineTo(px + cw, midY);
      ctx.lineTo(px, midY);
      ctx.closePath();
      const gr = ctx.createLinearGradient(0, ct, 0, ct + ch);
      gr.addColorStop(0, 'rgba(184, 154, 90, 0.2)');
      gr.addColorStop(0.5, 'rgba(184, 154, 90, 0.02)');
      gr.addColorStop(1, 'rgba(90, 138, 184, 0.15)');
      ctx.fillStyle = gr;
      ctx.fill();

      // Stroke
      ctx.beginPath();
      for (let i = 0; i < np; i++) {
        const x = px + (i / (np - 1)) * cw;
        const y = midY - (resp[i] / dBRange) * ch;
        if (i === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
      }
      ctx.strokeStyle = 'rgba(232, 228, 223, 0.7)';
      ctx.lineWidth = 1.5;
      ctx.stroke();

      // Zero line
      ctx.beginPath();
      ctx.moveTo(px, midY);
      ctx.lineTo(px + cw, midY);
      ctx.strokeStyle = 'rgba(120, 117, 111, 0.2)';
      ctx.lineWidth = 0.5;
      ctx.setLineDash([2, 2]);
      ctx.stroke();
      ctx.setLineDash([]);
    }

    // Dimension bars
    const barY = 74, barH = 6, barPad = 8, totalW = w - 16, barGap = 4;
    const barW = (totalW - barGap * 5) / 6;

    DIMENSIONS.forEach((dim, i) => {
      const x = barPad + i * (barW + barGap);
      const val = state[dim];
      const col = DIMENSION_COLORS[dim];

      // Background
      ctx.fillStyle = 'rgba(255, 255, 255, 0.06)';
      ctx.fillRect(x, barY, barW, barH);

      // Fill
      const fillW = (Math.abs(val) / 100) * (barW / 2);
      if (val > 0) {
        ctx.fillStyle = col;
        ctx.fillRect(x + barW / 2, barY, fillW, barH);
      } else if (val < 0) {
        ctx.fillStyle = col;
        ctx.fillRect(x + barW / 2 - fillW, barY, fillW, barH);
      }

      // Center line
      ctx.fillStyle = 'rgba(255, 255, 255, 0.15)';
      ctx.fillRect(x + barW / 2 - 0.5, barY - 1, 1, barH + 2);

      // Label
      ctx.font = '7px "Commit Mono", monospace';
      ctx.fillStyle = 'rgba(255, 255, 255, 0.35)';
      ctx.textAlign = 'center';
      ctx.fillText(dim.substring(0, 3).toUpperCase(), x + barW / 2, barY + barH + 10);

      // Value
      ctx.font = '8px "Commit Mono", monospace';
      ctx.fillStyle = Math.abs(val) > 0 ? col : 'rgba(255, 255, 255, 0.2)';
      ctx.fillText(val > 0 ? '+' + val : '' + val, x + barW / 2, barY + barH + 20);
    });

    ctx.textAlign = 'left';

    // Bypass overlay
    if (bypassed) {
      ctx.fillStyle = 'rgba(0, 0, 0, 0.7)';
      ctx.fillRect(0, 0, w, h);
      ctx.font = '14px "Commit Mono", monospace';
      ctx.fillStyle = 'rgba(184, 90, 90, 0.8)';
      ctx.textAlign = 'center';
      ctx.fillText('BYPASS', w / 2, h / 2 + 4);
      ctx.textAlign = 'left';
    }
  }, [state, bypassed, analyserRef, filtersRef, presetName]);

  useAnimationLoop(draw);

  return (
    <div
      className="w-[288px] h-[170px] rounded relative mb-5 z-[2] overflow-hidden"
      style={{
        background: '#000',
        boxShadow: 'inset 0 0 12px #000, 0 0.5px 0 rgba(255,255,255,0.05)',
        border: '1px solid #0a0a0d',
      }}
    >
      {/* OLED glow overlay */}
      <div
        className="absolute inset-0 pointer-events-none z-10 rounded"
        style={{
          background: 'linear-gradient(180deg, rgba(255,255,255,0.02) 0%, transparent 40%)',
        }}
      />
      <canvas ref={canvasRef} className="w-full h-full block" />
    </div>
  );
}
