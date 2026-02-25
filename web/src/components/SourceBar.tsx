import { useRef } from 'react';

interface SourceBarProps {
  onMic: () => void;
  onFile: (file: File) => void;
  source: string;
  bypassed: boolean;
  onBypassToggle: () => void;
  inputAnalyserRef: React.RefObject<AnalyserNode | null>;
  onSettingsOpen: () => void;
}

export function SourceBar({ onMic, onFile, source, bypassed, onBypassToggle, onSettingsOpen }: SourceBarProps) {
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

      <div
        className={`flex items-center rounded-md border transition-all duration-300 ease-out ${
          source === 'mic'
            ? 'border-terracotta'
            : 'border-border hover:border-terracotta'
        }`}
      >
        <button
          className={`font-mono text-xs px-4 py-2 rounded-l-md transition-all duration-300 ease-out cursor-pointer whitespace-nowrap ${
            source === 'mic'
              ? 'bg-terracotta text-bg'
              : 'bg-surface-2 text-text hover:text-terracotta'
          }`}
          onClick={onMic}
        >
          Guitar In
        </button>
        <button
          className={`font-mono text-xs px-2 py-2 rounded-r-md border-l transition-all duration-300 ease-out cursor-pointer flex items-center ${
            source === 'mic'
              ? 'bg-terracotta/15 text-terracotta border-terracotta/30 hover:bg-terracotta/25'
              : 'bg-surface-2 text-text-dim border-border hover:text-terracotta'
          }`}
          onClick={onSettingsOpen}
          aria-label="Audio input settings"
          title="Choose input device"
        >
          <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
            <path d="M12.22 2h-.44a2 2 0 0 0-2 2v.18a2 2 0 0 1-1 1.73l-.43.25a2 2 0 0 1-2 0l-.15-.08a2 2 0 0 0-2.73.73l-.22.38a2 2 0 0 0 .73 2.73l.15.1a2 2 0 0 1 1 1.72v.51a2 2 0 0 1-1 1.74l-.15.09a2 2 0 0 0-.73 2.73l.22.38a2 2 0 0 0 2.73.73l.15-.08a2 2 0 0 1 2 0l.43.25a2 2 0 0 1 1 1.73V20a2 2 0 0 0 2 2h.44a2 2 0 0 0 2-2v-.18a2 2 0 0 1 1-1.73l.43-.25a2 2 0 0 1 2 0l.15.08a2 2 0 0 0 2.73-.73l.22-.39a2 2 0 0 0-.73-2.73l-.15-.08a2 2 0 0 1-1-1.74v-.5a2 2 0 0 1 1-1.74l.15-.09a2 2 0 0 0 .73-2.73l-.22-.38a2 2 0 0 0-2.73-.73l-.15.08a2 2 0 0 1-2 0l-.43-.25a2 2 0 0 1-1-1.73V4a2 2 0 0 0-2-2z" />
            <circle cx="12" cy="12" r="3" />
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
        <span className="text-[11px] text-text-mid overflow-hidden text-ellipsis whitespace-nowrap max-w-[200px] font-mono">
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
