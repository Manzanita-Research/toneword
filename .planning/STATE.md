# Project State

## Project Reference

See: .planning/PROJECT.md (updated 2026-02-24)

**Core value:** Musicians shape tone with their ears, not their eyes — TONEWORD makes EQ work the way guitarists already think about sound.
**Current focus:** All phases complete — TONEWORD v1 ready for release

## Current Position

Phase: 4 of 4 (Validation and Distribution)
Plan: 3 of 3
Status: Phase 4 complete — all phases done
Last activity: 2026-02-25 — Plan 04-03 complete (CI workflow validated and approved)

Progress: [██████████] 100%

## Performance Metrics

**Velocity:**
- Total plans completed: 18
- Average duration: ~7min
- Total execution time: ~111min

**By Phase:**

| Phase | Plans | Total | Avg/Plan |
|-------|-------|-------|----------|
| 0 | 4/4 | 20min | 5min |
| 1 | 3/3 | ~15min | ~5min |
| 2 | 3/3 | 17min | 6min |
| 3 | 4/4 | 40min | 10min |
| 4 | 3/3 | ~11min | ~4min |

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
- [Phase 4]: All CI/CD steps in single workflow file (build_and_test.yml)
- [Phase 4]: Signing/notarization conditional on secrets — graceful degradation without Apple Developer account
- [Phase 4]: auval requires sudo cp to /Library/Audio/Plug-Ins/Components/ for CI discovery
- [Phase 4]: Removed CLAP from distribution template (TONEWORD ships VST3, AU, Standalone only)
- [Phase 4]: GitHub Release on version tags (v*) via softprops/action-gh-release

### Pending Todos

None — all phases complete.

### Blockers/Concerns

- [Phase 4 open]: Apple Developer Program enrollment ($99/yr) needed for code signing and notarization. CI pipeline works without it (uploads unsigned artifacts). Add secrets when account is ready.
- [Phase 1 resolved]: Filter gain unit mismatch — addressed with dB-to-linear conversion test (passing)

## Session Continuity

Last session: 2026-02-25
Stopped at: All phases complete — TONEWORD v1 roadmap finished
Resume file: .planning/phases/04-validation-and-distribution/04-03-SUMMARY.md
