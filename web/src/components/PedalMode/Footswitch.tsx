interface FootswitchProps {
  bypassed: boolean;
  onToggle: () => void;
}

export function Footswitch({ bypassed, onToggle }: FootswitchProps) {
  return (
    <div className="mt-auto flex flex-col items-center gap-2.5 z-[2]">
      {/* LED indicator */}
      <div
        className="w-2.5 h-2.5 rounded-full transition-all duration-150"
        style={{
          background: bypassed ? '#1a0a0a' : '#e04040',
          border: '1px solid rgba(255,255,255,0.05)',
          boxShadow: bypassed
            ? 'none'
            : '0 0 8px rgba(224, 64, 64, 0.6), 0 0 20px rgba(224, 64, 64, 0.25)',
        }}
      />

      {/* Footswitch button */}
      <button
        className="w-[64px] h-[64px] rounded-full border-none cursor-pointer relative transition-all duration-100 active:translate-y-px"
        style={{
          background: 'radial-gradient(circle at 42% 38%, #4a4845, #2a2827 50%, #1a1816)',
          boxShadow: '0 4px 12px rgba(107, 58, 42, 0.15), 0 2px 4px rgba(0,0,0,0.3), inset 0 2px 0 rgba(255,255,255,0.08), inset 0 -2px 0 rgba(0,0,0,0.3)',
        }}
        onClick={onToggle}
      >
        {/* Highlight circle */}
        <div
          className="absolute rounded-full"
          style={{
            top: '50%',
            left: '50%',
            width: '30px',
            height: '30px',
            transform: 'translate(-50%, -50%)',
            background: 'radial-gradient(circle at 45% 40%, rgba(255,255,255,0.06), transparent)',
          }}
        />
      </button>
    </div>
  );
}
