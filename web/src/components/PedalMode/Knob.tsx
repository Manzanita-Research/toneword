import { useRef } from 'react';

interface KnobProps {
  value: number; // -100 to 100
  color: string;
  label: string;
  onChange: (value: number) => void;
  small?: boolean;
}

export function Knob({ value, color, label, onChange, small = false }: KnobProps) {
  const startRef = useRef<{ y: number; value: number } | null>(null);
  const angle = (value / 100) * 135; // -135 to +135 degrees

  const handlePointerDown = (e: React.PointerEvent<HTMLDivElement>) => {
    e.currentTarget.setPointerCapture(e.pointerId);
    startRef.current = { y: e.clientY, value };
  };

  const handlePointerMove = (e: React.PointerEvent<HTMLDivElement>) => {
    if (!startRef.current) return;
    const delta = (startRef.current.y - e.clientY) * 1.2;
    const startAngle = (startRef.current.value / 100) * 135;
    const newAngle = Math.max(-135, Math.min(135, startAngle + delta));
    onChange(Math.round((newAngle / 135) * 100));
  };

  const handlePointerUp = () => {
    startRef.current = null;
  };

  const size = small ? 'w-[44px] h-[44px]' : 'w-[62px] h-[62px]';
  const indicatorStyle = small
    ? { top: '5px', height: '12px' }
    : { top: '7px', height: '16px' };

  return (
    <div className="flex flex-col items-center gap-1.5">
      <div className="text-[8px] tracking-[0.15em] uppercase text-white/35 text-center">
        {label}
      </div>
      <div
        className={`${size} rounded-full relative cursor-grab active:cursor-grabbing select-none touch-none`}
        style={{
          background: 'radial-gradient(circle at 40% 35%, #3a3835, #1a1816 60%, #141210)',
          boxShadow: '0 3px 8px rgba(107, 58, 42, 0.2), 0 1px 2px rgba(0,0,0,0.4), inset 0 1px 0 rgba(255,255,255,0.1), inset 0 -1px 0 rgba(0,0,0,0.3)',
        }}
        onPointerDown={handlePointerDown}
        onPointerMove={handlePointerMove}
        onPointerUp={handlePointerUp}
      >
        {/* Indicator line */}
        <div
          className="absolute left-1/2 w-0.5 rounded-sm"
          style={{
            ...indicatorStyle,
            background: color,
            transformOrigin: 'bottom center',
            transform: `translateX(-50%) rotate(${angle}deg)`,
          }}
        />
        {/* Ring */}
        {!small && (
          <div
            className="absolute rounded-full pointer-events-none"
            style={{
              inset: '-4px',
              border: '1px solid rgba(255,255,255,0.04)',
            }}
          />
        )}
      </div>
    </div>
  );
}
