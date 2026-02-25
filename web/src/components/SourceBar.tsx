import { useRef } from 'react';

interface SourceBarProps {
  onMic: () => void;
  onFile: (file: File) => void;
  source: string;
  bypassed: boolean;
  onBypassToggle: () => void;
  inputAnalyserRef: React.RefObject<AnalyserNode | null>;
  onSettingsOpen: () => void;
  hasDeviceSelected: boolean;
}

export function SourceBar({ onMic, onFile, source, bypassed, onBypassToggle, onSettingsOpen, hasDeviceSelected }: SourceBarProps) {
  const fileInputRef = useRef<HTMLInputElement>(null);

  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (file) onFile(file);
  };

  return (
    <div className="flex items-center gap-3 mb-9 px-5 py-4 bg-surface border border-border rounded-lg flex-wrap" style={{ boxShadow: '0 2px 12px rgba(107, 58, 42, 0.06)' }}>
      <label className="text-[10px] tracking-[0.15em] uppercase text-text-dim whitespace-nowrap flex items-center gap-1.5">
        <span
          className={`inline-block w-1.5 h-1.5 rounded-full transition-colors duration-300 ${
            source ? 'bg-green-400 animate-pulse' : 'bg-text-dim'
          }`}
        />
        Source
      </label>

      <div className="flex items-center gap-1">
        <button
          className={`font-mono text-xs px-4 py-2 rounded-l-md border border-r-0 transition-all duration-300 ease-out cursor-pointer whitespace-nowrap ${
            source === 'mic'
              ? 'bg-terracotta text-bg border-terracotta'
              : 'bg-surface-2 border-border text-text hover:border-terracotta hover:text-terracotta'
          }`}
          onClick={onMic}
        >
          Mic / Guitar In
        </button>
        <button
          className={`font-mono text-xs px-2 py-2 rounded-r-md border transition-all duration-300 ease-out cursor-pointer ${
            hasDeviceSelected
              ? 'bg-surface-2 border-terracotta/50 text-terracotta hover:bg-terracotta/10'
              : 'bg-surface-2 border-border text-text-dim hover:border-terracotta hover:text-terracotta'
          }`}
          onClick={onSettingsOpen}
          aria-label="Audio input settings"
          title="Choose input device"
        >
          <svg width="14" height="14" viewBox="0 0 16 16" fill="none" stroke="currentColor" strokeWidth="1.5" strokeLinecap="round" strokeLinejoin="round">
            <circle cx="8" cy="8" r="2.5" />
            <path d="M8 1.5v1.2M8 13.3v1.2M1.5 8h1.2M13.3 8h1.2M3.4 3.4l.85.85M11.75 11.75l.85.85M3.4 12.6l.85-.85M11.75 4.25l.85-.85" />
          </svg>
        </button>
      </div>

      <button
        className={`font-mono text-xs px-4 py-2 rounded-md border transition-all duration-300 ease-out cursor-pointer whitespace-nowrap ${
          source && source !== 'mic'
            ? 'bg-terracotta text-bg border-terracotta'
            : 'bg-surface-2 border-border text-text hover:border-terracotta hover:text-terracotta'
        }`}
        onClick={() => fileInputRef.current?.click()}
      >
        Load Audio
      </button>

      <input
        ref={fileInputRef}
        type="file"
        accept="audio/*"
        className="hidden"
        onChange={handleFileChange}
      />

      {source && source !== 'mic' && (
        <span className="text-[11px] text-text-mid overflow-hidden text-ellipsis whitespace-nowrap max-w-[200px] font-body">
          {source}
        </span>
      )}

      <div className="flex-1" />

      <button
        className={`font-mono text-[10px] tracking-[0.1em] uppercase px-4 py-2 rounded-md border cursor-pointer transition-all duration-300 ease-out ml-auto whitespace-nowrap ${
          bypassed
            ? 'bg-bite text-white border-bite'
            : 'bg-surface-2 border-border text-text hover:border-bite hover:text-bite'
        }`}
        onClick={onBypassToggle}
      >
        {bypassed ? 'Bypassed' : 'Bypass'}
      </button>
    </div>
  );
}
