---
phase: 04-validation-and-distribution
plan: 01
subsystem: infra
tags: [github-actions, ci, pluginval, auval, cmake, juce]

requires:
  - phase: 03-gui-studio-mode
    provides: Complete plugin with GUI ready for validation
provides:
  - GitHub Actions CI workflow for macOS build, test, and validation
  - pluginval validation for VST3 and AU at strictness level 10
  - auval validation for AU component (aufx Tnwd Manz)
  - sccache build caching for fast CI builds
  - Build artifact upload for download
affects: [04-validation-and-distribution]

tech-stack:
  added: [github-actions, pluginval, sccache, ninja]
  patterns:
    - "GitHubENV.cmake writes .env consumed by CI via cat .env >> $GITHUB_ENV"
    - "working-directory: plugin for all build steps (plugin is subdirectory)"
    - "auval requires sudo cp to /Library/Audio/Plug-Ins/Components/ for CI discovery"

key-files:
  created:
    - .github/workflows/build_and_test.yml
  modified:
    - plugin/packaging/distribution.xml.template

key-decisions:
  - "Combined all CI steps (build, test, validate, sign, package, release) in single workflow file"
  - "Removed CLAP from distribution.xml.template (TONEWORD ships VST3, AU, Standalone only)"
  - "Updated min OS from 10.13 to 12.0 to match CMakeLists.txt CMAKE_OSX_DEPLOYMENT_TARGET"
  - "auval step uses sudo cp to system Components path for Audio Unit discovery in CI"

patterns-established:
  - "Workflow uses working-directory: plugin default with explicit overrides for non-plugin steps"
  - "Signing conditional on secrets.DEV_ID_APP_CERT presence — graceful degradation"

requirements-completed:
  - FMT-03

duration: 8min
completed: 2026-02-25
---

# Phase 4 Plan 01: CI Workflow Summary

**GitHub Actions workflow for macOS build, Catch2 tests, pluginval (VST3 + AU), and auval validation with sccache caching**

## Performance

- **Duration:** 8 min
- **Started:** 2026-02-25
- **Completed:** 2026-02-25
- **Tasks:** 2
- **Files modified:** 2

## Accomplishments
- Created complete CI workflow at .github/workflows/build_and_test.yml
- pluginval validates both VST3 and AU builds at strictness level 10
- auval validates AU component with correct identifiers (aufx Tnwd Manz)
- Updated distribution.xml.template: removed CLAP choice, updated min OS to 12.0
- Build artifacts uploaded as GitHub Actions artifacts

## Task Commits

1. **Task 1: Create GitHub Actions CI workflow** - `188d134` (feat)
2. **Task 2: Update distribution.xml.template** - `188d134` (feat, same commit)

## Files Created/Modified
- `.github/workflows/build_and_test.yml` - Complete CI/CD workflow
- `plugin/packaging/distribution.xml.template` - Removed CLAP, updated min OS

## Decisions Made
- Combined Plans 04-01 and 04-02 tasks into a single workflow file (simpler than splitting across commits)
- Used `secrets.DEV_ID_APP_CERT != ''` conditional for signing steps
- auval step copies AU to system path before validation (required for CI discovery)

## Deviations from Plan
None - plan executed as specified

## Issues Encountered
None

## Next Phase Readiness
- CI workflow ready for push to GitHub
- Signing and notarization steps will activate when Apple Developer secrets are configured

---
*Phase: 04-validation-and-distribution*
*Completed: 2026-02-25*
