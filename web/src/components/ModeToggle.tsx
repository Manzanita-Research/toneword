interface ModeToggleProps {
  mode: 'studio' | 'pedal';
  onModeChange: (mode: 'studio' | 'pedal') => void;
}

export function ModeToggle({ mode, onModeChange }: ModeToggleProps) {
  return (
    <div className="fixed top-0 left-0 right-0 z-50 flex justify-center p-3 bg-bg/92 backdrop-blur-xl border-b border-border">
      <div className="flex bg-surface border border-border rounded-lg overflow-hidden">
        <button
          className={`font-mono text-[11px] tracking-[0.1em] uppercase px-5 py-2 border-none cursor-pointer transition-all duration-200 ${
            mode === 'studio'
              ? 'bg-body text-bg'
              : 'bg-transparent text-text-dim hover:text-text'
          }`}
          onClick={() => onModeChange('studio')}
        >
          Studio
        </button>
        <button
          className={`font-mono text-[11px] tracking-[0.1em] uppercase px-5 py-2 border-none cursor-pointer transition-all duration-200 ${
            mode === 'pedal'
              ? 'bg-body text-bg'
              : 'bg-transparent text-text-dim hover:text-text'
          }`}
          onClick={() => onModeChange('pedal')}
        >
          Pedal
        </button>
      </div>
    </div>
  );
}
