# Requirements: TONEWORD VST Plugin

**Defined:** 2026-02-24
**Core Value:** Musicians shape tone with their ears, not their eyes — TONEWORD makes EQ work the way guitarists already think about sound.

## v1 Requirements

Requirements for initial release. Each maps to roadmap phases.

### Web Reference Build

- [ ] **WEB-01**: Vite + React + Tailwind project replacing single-file HTML prototype
- [ ] **WEB-02**: All audio processing (11-band EQ, gain mappings, tanh clipping, presets) works identically to original prototype
- [ ] **WEB-03**: Studio mode and pedal mode both render with Manzanita frontend design applied
- [ ] **WEB-04**: Colored slider handles display correctly across Chrome, Safari, and Firefox
- [ ] **WEB-05**: Manual UAT confirms tone shaping, visual feedback, and mode switching all work as expected

### DSP Engine

- [ ] **DSP-01**: Plugin processes audio in real-time with no glitches at standard buffer sizes (128-512 samples)
- [ ] **DSP-02**: Six semantic dimensions (Warmth, Bite, Air, Body, Glass, Velvet) each control an 11-band parametric EQ via weighted gain mappings ported faithfully from the HTML prototype
- [ ] **DSP-03**: Bipolar parameter range (-100 to +100) for each dimension
- [ ] **DSP-04**: Cross-coupled negative weights between opposing dimensions (e.g., Glass cuts Velvet frequencies)
- [ ] **DSP-05**: tanh soft-clipping prevents extreme gains when stacking multiple dimensions
- [ ] **DSP-06**: Filter gain values correctly converted from dB to linear multipliers for JUCE IIR coefficients
- [ ] **DSP-07**: SmoothedValue applied to all six parameters to prevent zipper noise during automation

### Plugin Format

- [ ] **FMT-01**: Builds as VST3 plugin loadable in Ableton, Reaper, FL Studio, Bitwig
- [ ] **FMT-02**: Builds as AU plugin loadable in Logic Pro
- [ ] **FMT-03**: Plugin passes pluginval validation without errors
- [ ] **FMT-04**: Plugin bypass passes clean signal with no artifacts

### State & Presets

- [x] **STA-01**: All six dimension values persist when DAW project is saved and reloaded
- [x] **STA-02**: Eight factory tone recipe presets ship with the plugin (Nashville Twang, Midnight Jazz, Punk Scoop, etc.)
- [x] **STA-03**: User can browse and load factory presets from the plugin UI
- [x] **STA-04**: All six dimensions are exposed as named automatable DAW parameters
- [x] **STA-05**: Undo/redo works for parameter changes via DAW gesture system

### GUI — Studio Mode

- [x] **GUI-01**: Six semantic dimension controls with labeled bipolar range (-100 to +100)
- [x] **GUI-02**: Composite EQ curve visualization showing the combined effect of all dimensions
- [x] **GUI-03**: Real-time spectrum analyzer (pre/post EQ)
- [ ] **GUI-04**: Teaching layer: read-only parameter table showing actual frequency/gain/Q values for all 11 bands
- [ ] **GUI-05**: Plugin window supports HiDPI/Retina displays without blurriness
- [ ] **GUI-06**: Plugin UI is resizable

## v2 Requirements

Deferred to future release. Tracked but not in current roadmap.

### Pedal Mode

- **PED-01**: Pedal mode UI with rotary knobs and boutique hardware aesthetic (Chase Bliss / Strymon)
- **PED-02**: OLED-style display showing active preset and dimension values
- **PED-03**: Bidirectional sync between studio and pedal modes

### Extended Features

- **EXT-01**: A/B snapshot comparison for quick before/after
- **EXT-02**: User-created preset save/load
- **EXT-03**: CLAP plugin format support

## Out of Scope

Explicitly excluded. Documented to prevent scope creep.

| Feature | Reason |
|---------|--------|
| Full parametric EQ view with draggable bands | Undermines core thesis — the semantic layer IS the interface, not a wrapper on a parametric EQ |
| AI/auto-analysis mode | Contradicts "ears, not eyes" — the user shapes tone manually |
| Dynamic EQ (frequency-reactive gain) | Changes the static gainMap architecture; separate product concept |
| Conversational text-prompt interface | Significant NLP complexity; the six bipolar dimensions are the v1 semantic vocabulary |
| Custom user-defined tone words | Deferred until v1 validates the core six dimensions |
| MIDI/hardware control mapping | Deferred to physical stompbox milestone (Daisy Seed) |
| Physical stompbox | Separate future milestone after VST validates with DAW users |
| AAX format | Requires Avid partnership + iLok; not worth the overhead for v1 |
| Spectral metering / LUFS | Not a mastering tool — use a dedicated metering plugin |

## Traceability

Which phases cover which requirements. Updated during roadmap creation.

| Requirement | Phase | Status |
|-------------|-------|--------|
| WEB-01 | Phase 0 | Pending |
| WEB-02 | Phase 0 | Pending |
| WEB-03 | Phase 0 | Pending |
| WEB-04 | Phase 0 | Pending |
| WEB-05 | Phase 0 | Pending |
| DSP-01 | Phase 1 | Pending |
| DSP-02 | Phase 1 | Pending |
| DSP-03 | Phase 1 | Pending |
| DSP-04 | Phase 1 | Pending |
| DSP-05 | Phase 1 | Pending |
| DSP-06 | Phase 1 | Pending |
| DSP-07 | Phase 1 | Pending |
| FMT-01 | Phase 1 | Pending |
| FMT-02 | Phase 1 | Pending |
| FMT-03 | Phase 4 | Pending |
| FMT-04 | Phase 1 | Pending |
| STA-01 | Phase 2 | Complete |
| STA-02 | Phase 2 | Complete |
| STA-03 | Phase 2 | Complete |
| STA-04 | Phase 2 | Complete |
| STA-05 | Phase 2 | Complete |
| GUI-01 | Phase 3 | Complete |
| GUI-02 | Phase 3 | Complete |
| GUI-03 | Phase 3 | Complete |
| GUI-04 | Phase 3 | Pending |
| GUI-05 | Phase 3 | Pending |
| GUI-06 | Phase 3 | Pending |

**Coverage:**
- v1 requirements: 27 total
- Mapped to phases: 27
- Unmapped: 0 ✓

---
*Requirements defined: 2026-02-24*
*Last updated: 2026-02-24 after roadmap creation*
