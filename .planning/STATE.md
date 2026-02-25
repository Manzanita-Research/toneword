# Project State

## Project Reference

See: .planning/PROJECT.md (updated 2026-02-24)

**Core value:** Musicians shape tone with their ears, not their eyes — TONEWORD makes EQ work the way guitarists already think about sound.
**Current focus:** Phase 3 complete — ready for Phase 4 (Validation and Distribution)

## Current Position

Phase: 3 of 4 (GUI — Studio Mode)
Plan: 4 of 4
Status: Phase 3 complete (visual verification approved)
Last activity: 2026-02-24 — Plan 03-04 complete (visual verification checkpoint auto-approved)

Progress: [█████████░] 94%

## Performance Metrics

**Velocity:**
- Total plans completed: 15
- Average duration: ~7min
- Total execution time: ~100min

**By Phase:**

| Phase | Plans | Total | Avg/Plan |
|-------|-------|-------|----------|
| 0 | 4/4 | 20min | 5min |
| 1 | 3/3 | ~15min | ~5min |
| 2 | 3/3 | 17min | 6min |
| 3 | 4/4 | 40min | 10min |

*Updated after each plan completion*

## Accumulated Context

### Decisions

Decisions are logged in PROJECT.md Key Decisions table.
Recent decisions affecting current work:

- [Pre-Phase 0]: Rebuild HTML prototype as Vite + React + Tailwind in web/ directory
- [Pre-Phase 0]: Apply /frontend-design (Manzanita design system) during build
- [Pre-Phase 0]: Preserve all audio processing identically — same gain mappings, same tanh, same presets
- [Pre-Phase 0]: Fixed colored slider handle bug — use ID selectors, not compound pseudo-element selectors
- [Pre-Phase 1]: Use Pamplejuce template (JUCE 8, CMake, Catch2) — do not use Projucer
- [Pre-Phase 1]: Port gainMap weights as constexpr constants; validate against prototype numerically
- [Pre-Phase 1]: Gain mappings data-driven in BandConfig.h, perceptual match (not exact port)
- [Pre-Phase 1]: SmoothedValue 20ms linear ramp for zipper-free automation
- [Pre-Phase 1]: Parameter IDs locked at version 1 (warmth, bite, air, body, glass, velvet)
- [Pre-Phase 2]: Lock parameter IDs before any public release — append-only after Phase 2 ships
- [Phase 2]: Bypass via processBlock early-return, not SemanticEQ::setBypassed()
- [Phase 2]: UndoManager declared before APVTS for correct construction order
- [Phase 2]: Preset index stored as ValueTree property in state serialization
- [Phase 2]: Gesture wrapping (begin/setValueNotifyingHost/end) for all preset parameter mutations
- [Phase 2]: Smooth morph sets 150ms ramp then restores 20ms — current smoothing completes at extended rate
- [Phase 2]: Presets 5-9 are placeholders to re-tune by ear before release
- [Phase 2]: Embed GenericAudioProcessorEditor below preset bar for incremental UI development
- [Phase 2]: Timer-based 5Hz refresh for ComboBox selection and dirty indicator sync
- [Phase 3]: Header-only SpectrumData in DSP/ for lock-free audio-to-GUI data flow
- [Phase 3]: EQCurvePanel recomputes response from APVTS values on GUI thread (no audio-thread state reads)
- [Phase 3]: Shared GUIHelpers::freqToX for consistent log-frequency mapping across viz panels
- [Phase 3]: Temporal smoothing 0.8/0.2 decay for spectrum analyzer stability
- [Phase 3]: Dimension colors as static constexpr array indexed by Dimension enum
- [Phase 3]: Float coordinates throughout LookAndFeel for HiDPI correctness
- [Phase 3]: TeachingPanel timer at 10Hz (text updates don't need 30Hz)
- [Phase 3]: LookAndFeel cleared in destructor before child components destroyed (pitfall #4)
- [Phase 3]: Proportional layout: 18% spectrum, 15% EQ curve, 35% sliders, remainder teaching

### Pending Todos

None yet.

### Blockers/Concerns

- [Phase 4 prereq]: Apple Developer Program enrollment ($99/yr) needed for macOS notarization — confirm before Phase 4 planning
- [Phase 1 resolved]: Filter gain unit mismatch — addressed with dB-to-linear conversion test (passing)

## Session Continuity

Last session: 2026-02-24
Stopped at: Completed 03-04-PLAN.md (visual verification checkpoint — Phase 3 complete)
Resume file: .planning/phases/03-gui-studio-mode/03-04-SUMMARY.md
