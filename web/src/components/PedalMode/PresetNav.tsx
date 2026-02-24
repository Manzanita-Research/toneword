interface PresetNavProps {
  currentPresetName: string;
  onPrev: () => void;
  onNext: () => void;
}

export function PresetNav({ currentPresetName, onPrev, onNext }: PresetNavProps) {
  return (
    <div className="flex items-center gap-3 mb-3 z-[2]">
      <button
        className="w-[26px] h-[26px] rounded-full border-none text-white/40 text-[13px] cursor-pointer flex items-center justify-center transition-all duration-150 hover:text-body active:scale-95"
        style={{
          background: 'radial-gradient(circle at 40% 35%, #2a2827, #161514)',
          boxShadow: '0 2px 4px rgba(107, 58, 42, 0.12), inset 0 1px 0 rgba(255,255,255,0.06)',
        }}
        onClick={onPrev}
      >
        &#9666;
      </button>

      <span className="text-[10px] tracking-[0.12em] uppercase text-white/50 min-w-[100px] text-center">
        {currentPresetName}
      </span>

      <button
        className="w-[26px] h-[26px] rounded-full border-none text-white/40 text-[13px] cursor-pointer flex items-center justify-center transition-all duration-150 hover:text-body active:scale-95"
        style={{
          background: 'radial-gradient(circle at 40% 35%, #2a2827, #161514)',
          boxShadow: '0 2px 4px rgba(107, 58, 42, 0.12), inset 0 1px 0 rgba(255,255,255,0.06)',
        }}
        onClick={onNext}
      >
        &#9656;
      </button>
    </div>
  );
}
