import { useRef } from 'react';

interface SourceBarProps {
  onMic: () => void;
  onFile: (file: File) => void;
  source: string;
  bypassed: boolean;
  onBypassToggle: () => void;
  inputAnalyserRef: React.RefObject<AnalyserNode | null>;
}

export function SourceBar({ onMic, onFile, source, bypassed, onBypassToggle }: SourceBarProps) {
  const fileInputRef = useRef<HTMLInputElement>(null);

  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (file) onFile(file);
  };

  return (
    <div className="flex items-center gap-3 mb-9 px-5 py-4 bg-surface border border-border rounded-lg flex-wrap">
      <label className="text-[10px] tracking-[0.15em] uppercase text-text-dim whitespace-nowrap flex items-center gap-1.5">
        <span
          className={`inline-block w-1.5 h-1.5 rounded-full transition-colors duration-300 ${
            source ? 'bg-green-400 animate-pulse' : 'bg-text-dim'
          }`}
        />
        Source
      </label>

      <button
        className={`font-mono text-xs px-4 py-2 rounded-md border transition-all duration-200 cursor-pointer whitespace-nowrap ${
          source === 'mic'
            ? 'bg-body text-bg border-body'
            : 'bg-surface-2 border-border text-text hover:border-body hover:text-body'
        }`}
        onClick={onMic}
      >
        Mic / Guitar In
      </button>

      <button
        className={`font-mono text-xs px-4 py-2 rounded-md border transition-all duration-200 cursor-pointer whitespace-nowrap ${
          source && source !== 'mic'
            ? 'bg-body text-bg border-body'
            : 'bg-surface-2 border-border text-text hover:border-body hover:text-body'
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
        <span className="text-[11px] text-text-mid overflow-hidden text-ellipsis whitespace-nowrap max-w-[200px]">
          {source}
        </span>
      )}

      <div className="flex-1" />

      <button
        className={`font-mono text-[10px] tracking-[0.1em] uppercase px-4 py-2 rounded-md border cursor-pointer transition-all duration-200 ml-auto whitespace-nowrap ${
          bypassed
            ? 'bg-bite text-white border-bite'
            : 'bg-surface-2 border-border text-text hover:border-body hover:text-body'
        }`}
        onClick={onBypassToggle}
      >
        {bypassed ? 'Bypassed' : 'Bypass'}
      </button>
    </div>
  );
}
