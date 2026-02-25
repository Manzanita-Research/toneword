# Roadmap: TONEWORD VST Plugin

## Overview

Rebuild the working TONEWORD HTML prototype as a proper Vite web app (Phase 0), then port to a JUCE-based VST3/AU plugin. The build order follows hard dependencies: get the web reference implementation clean first, then DSP engine, state, GUI, and distribution.

## Phases

**Phase Numbering:**
- Integer phases (1, 2, 3): Planned milestone work
- Decimal phases (2.1, 2.2): Urgent insertions (marked with INSERTED)

Decimal phases appear between their surrounding integers in numeric order.

- [x] **Phase 0: Web Reference Build** - Rebuild single-file HTML prototype as a Vite app with proper structure and Manzanita frontend design (completed 2026-02-24)
- [x] **Phase 1: DSP Engine and Plugin Scaffold** - JUCE project + working audio processing, no GUI required (completed 2026-02-24)
- [x] **Phase 2: State, Presets, and Parameter Model** - DAW save/recall, factory presets, stable parameter IDs (completed 2026-02-25)
- [x] **Phase 3: GUI — Studio Mode** - Full visual interface: controls, EQ curve, spectrum analyzer, teaching layer (completed 2026-02-24)
- [x] **Phase 4: Validation and Distribution** - pluginval CI, macOS code signing, notarized release package (completed 2026-02-25)
- [ ] **Phase 5: CI and Packaging Fixes** - Fix GitHub Actions versions and missing packaging resource (gap closure)
- [ ] **Phase 6: Retroactive Verification Documentation** - Create missing VERIFICATION.md files and update all requirement checkboxes (gap closure)

## Phase Details

### Phase 0: Web Reference Build
**Goal**: Rebuild the single-file HTML prototype as a proper Vite + React + Tailwind web app with Manzanita frontend design system, producing a clean reference implementation that validates all UI/UX before the JUCE port
**Depends on**: Nothing (first phase)
**Requirements**: WEB-01, WEB-02, WEB-03, WEB-04, WEB-05
**Success Criteria** (what must be TRUE):
  1. All six semantic dimensions work identically to the original prototype — same gain mappings, same audio processing, same presets
  2. Studio mode and pedal mode render correctly with Manzanita design language applied
  3. Colored slider handles display correctly across Chrome, Safari, and Firefox
  4. Manual UAT confirms the app feels right — tone shaping, visual feedback, mode switching all work as expected
  5. Codebase is structured in maintainable components (not a single file)
**Plans:** 4 plans

Plans:
- [x] 00-01-PLAN.md — Scaffold Vite project + audio engine + types + hooks + app shell
- [x] 00-02-PLAN.md — Studio mode UI (sliders, spectrum, EQ curve, presets, param table)
- [x] 00-03-PLAN.md — Pedal mode UI (knobs, OLED, footswitch, preset nav)
- [x] 00-04-PLAN.md — Manual UAT checkpoint

### Phase 1: DSP Engine and Plugin Scaffold
**Goal**: A JUCE plugin that loads in DAWs, processes audio through the correct 11-band semantic EQ, and passes audio without glitches
**Depends on**: Phase 0 (reference implementation for validation)
**Requirements**: DSP-01, DSP-02, DSP-03, DSP-04, DSP-05, DSP-06, DSP-07, FMT-01, FMT-02, FMT-04
**Success Criteria** (what must be TRUE):
  1. Plugin loads in Ableton (VST3) and Logic Pro (AU) without errors
  2. Moving a dimension slider changes the audible character of a guitar signal in the expected direction — Warmth adds low-mid body, Bite adds presence, Air opens the top end
  3. Stacking all six dimensions at maximum produces no extreme gain spikes — tanh soft-clipping keeps output within a reasonable range
  4. Plugin passes audio cleanly at 128 and 512 sample buffer sizes with no dropouts or crackle
  5. Plugin bypass passes a clean, unprocessed signal with no artifacts
**Plans:** 3 plans

Plans:
- [x] 01-01-PLAN.md — Scaffold Pamplejuce project + configure CMake for Toneword (VST3, AU, Standalone)
- [x] 01-02-PLAN.md — TDD: DSP engine (gain calculation, 11-band filter chain, tanh clipping, dB-to-linear conversion)
- [x] 01-03-PLAN.md — Wire DSP into AudioProcessor with APVTS parameters + SmoothedValue + DAW verification

### Phase 2: State, Presets, and Parameter Model
**Goal**: DAW project save/recall works, all eight factory presets are loadable, and all six dimensions are automatable DAW parameters — with parameter IDs locked for all future releases
**Depends on**: Phase 1
**Requirements**: STA-01, STA-02, STA-03, STA-04, STA-05
**Success Criteria** (what must be TRUE):
  1. Save a project in Logic with all dimensions set, close and reopen — values are exactly what was saved
  2. All eight factory tone recipes appear in the preset browser and load with the correct dimension values
  3. Recording automation on a dimension parameter in Reaper plays back the parameter movement accurately
  4. Undo/redo via the DAW's native undo history reverts and replays dimension changes
**Plans:** 3 plans

Plans:
- [x] 02-01-PLAN.md — Add bypass + snap/smooth parameters, wire UndoManager, update state serialization
- [x] 02-02-PLAN.md — Factory presets (9 total), PresetManager, program interface for DAW preset browsers
- [x] 02-03-PLAN.md — Preset browser UI (ComboBox + arrows + dirty indicator) + DAW verification

### Phase 3: GUI — Studio Mode
**Goal**: The full visual interface is built: six labeled dimension controls, a composite EQ curve, a real-time spectrum analyzer, and the teaching layer showing actual parametric values — all rendering sharply on Retina displays
**Depends on**: Phase 2
**Requirements**: GUI-01, GUI-02, GUI-03, GUI-04, GUI-05, GUI-06
**Success Criteria** (what must be TRUE):
  1. Six dimension controls are visible, labeled, and move the EQ curve in real-time as they're adjusted
  2. The EQ curve display shows the composite frequency response of all six dimensions combined
  3. The spectrum analyzer shows pre/post EQ signal in real-time while audio plays
  4. The teaching layer shows the actual frequency, gain, and Q values for all 11 bands, updating as dimensions change
  5. Plugin UI renders sharply on a Retina/HiDPI display with no blur or pixel doubling, and the window is resizable
**Plans:** 4/4 plans executed

Plans:
- [x] 03-01-PLAN.md — LookAndFeel + dimension slider component + 2x3 slider grid
- [x] 03-02-PLAN.md — Spectrum analyzer (FFT FIFO) + EQ curve visualization
- [x] 03-03-PLAN.md — Teaching panel + editor assembly (replace GenericEditor) + resizable/HiDPI
- [x] 03-04-PLAN.md — Visual verification checkpoint

### Phase 4: Validation and Distribution
**Goal**: The plugin passes format validation, is code-signed and notarized for macOS, and can be installed by someone who doesn't build it themselves
**Depends on**: Phase 3
**Requirements**: FMT-03
**Success Criteria** (what must be TRUE):
  1. pluginval runs against both VST3 and AU builds in CI and reports zero errors
  2. A colleague on a different Mac can install the plugin and open it in Logic without a Gatekeeper warning
  3. The AU build passes `auval` without errors
**Plans:** 3 plans

Plans:
- [x] 04-01-PLAN.md — CI workflow: build, test, pluginval (VST3 + AU), auval, artifact upload
- [x] 04-02-PLAN.md — Code signing, notarization, .pkg packaging, GitHub Release
- [x] 04-03-PLAN.md — Workflow validation and human review checkpoint

### Phase 5: CI and Packaging Fixes
**Goal**: Fix CI workflow bugs that prevent GitHub Actions from running, and resolve missing packaging resource so distribution pipeline works end-to-end
**Depends on**: Phase 4
**Requirements**: FMT-03
**Gap Closure:** Closes integration gaps and conditional flow from v1.0 audit
**Success Criteria** (what must be TRUE):
  1. GitHub Actions workflow uses valid action versions (@v4) and parses without errors
  2. `background.png` exists in packaging resources OR is removed from distribution.xml.template
  3. CI workflow can be pushed and run without immediate failures
**Plans:** 1 plan

Plans:
- [ ] 05-01-PLAN.md — Fix missing background.png, rename pamplejuce.icns to toneword.icns, update workflow reference

### Phase 6: Retroactive Verification Documentation
**Goal**: Create formal VERIFICATION.md artifacts for Phase 1 and Phase 4, and update all requirement checkboxes in REQUIREMENTS.md to reflect actual completion status
**Depends on**: Phase 5
**Requirements**: DSP-01, DSP-02, DSP-03, DSP-04, DSP-05, DSP-06, DSP-07, FMT-01, FMT-02, FMT-03, FMT-04, WEB-01, WEB-02, WEB-03, WEB-04, WEB-05
**Gap Closure:** Closes all 11 orphaned requirements and 5 checkbox-only gaps from v1.0 audit
**Success Criteria** (what must be TRUE):
  1. Phase 1 VERIFICATION.md exists documenting all 10 requirements with evidence from integration checker and user DAW approval
  2. Phase 4 VERIFICATION.md exists documenting FMT-03 with evidence from summaries and CI workflow review
  3. All 27 REQUIREMENTS.md checkboxes reflect actual status (16 currently unchecked requirements marked complete)
  4. Traceability table status column updated for all gap closure requirements
**Plans:** TBD

## Progress

**Execution Order:**
Phases execute in numeric order: 0 → 1 → 2 → 3 → 4

| Phase | Plans Complete | Status | Completed |
|-------|----------------|--------|-----------|
| 0. Web Reference Build | 4/4 | Complete | 2026-02-24 |
| 1. DSP Engine and Plugin Scaffold | 3/3 | Complete | 2026-02-24 |
| 2. State, Presets, and Parameter Model | 3/3 | Complete | 2026-02-25 |
| 3. GUI — Studio Mode | 4/4 | Complete | 2026-02-24 |
| 4. Validation and Distribution | 3/3 | Complete | 2026-02-25 |
| 5. CI and Packaging Fixes | 0/1 | Planned | — |
| 6. Retroactive Verification Docs | 0/? | Pending | — |
