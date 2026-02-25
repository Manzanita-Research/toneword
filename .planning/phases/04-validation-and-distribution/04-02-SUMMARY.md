---
phase: 04-validation-and-distribution
plan: 02
subsystem: infra
tags: [codesign, notarization, pkgbuild, productbuild, github-releases]

requires:
  - phase: 04-validation-and-distribution
    provides: CI workflow base from Plan 04-01
provides:
  - Conditional code signing with Developer ID Application certificate
  - Notarization via notarytool with stapling
  - .pkg installer (VST3 + AU + Standalone) via pkgbuild + productbuild
  - GitHub Release creation on version tags
  - Unsigned artifact fallback when no signing secrets
affects: []

tech-stack:
  added: [sudara/basic-macos-keychain-action, softprops/action-gh-release, notarytool, pkgbuild, productbuild]
  patterns:
    - "Signing conditional on steps.keychain.outcome == 'success'"
    - "pkgbuild creates component packages, productbuild wraps with distribution.xml"
    - "notarytool submit --wait + stapler staple for offline notarization"

key-files:
  created: []
  modified:
    - .github/workflows/build_and_test.yml

key-decisions:
  - "Signing and packaging steps integrated into same workflow file as build/test"
  - "Release job triggers on tags/v* pattern, creates prerelease by default"
  - "Unsigned artifacts uploaded as fallback when no Apple Developer secrets"

patterns-established:
  - "Version tag workflow: git tag v0.1.0 && git push --tags triggers release"
  - "Secrets gating: all signing/notarization steps skip gracefully without secrets"

requirements-completed:
  - FMT-03

duration: 0min
completed: 2026-02-25
---

# Phase 4 Plan 02: Signing, Packaging, and Release Summary

**Conditional code signing, notarization, .pkg packaging, and GitHub Release integrated into CI workflow**

## Performance

- **Duration:** 0 min (combined with Plan 04-01 execution)
- **Started:** 2026-02-25
- **Completed:** 2026-02-25
- **Tasks:** 2
- **Files modified:** 1

## Accomplishments
- Code signing steps using sudara/basic-macos-keychain-action for certificate import
- Codesign VST3, AU, and Standalone with --deep --strict --options=runtime --timestamp
- pkgbuild + productbuild creates .pkg installer with VST3, AU, Standalone choices
- Notarization via notarytool with --wait and stapler staple
- GitHub Release job on version tags with .pkg as downloadable asset
- Graceful fallback: unsigned artifacts uploaded when no signing secrets

## Task Commits

1. **Task 1: Code signing and notarization** - `188d134` (feat, combined with Plan 01)
2. **Task 2: GitHub Release job** - `188d134` (feat, combined with Plan 01)

## Files Created/Modified
- `.github/workflows/build_and_test.yml` - Added signing, packaging, notarization, release sections

## Decisions Made
- Combined with Plan 04-01 into single commit since all steps are in the same file
- Custom icons step included for VST3 and AU bundles (uses pamplejuce.icns)

## Deviations from Plan
- Plans 04-01 and 04-02 executed as a single workflow file creation rather than sequential modifications (simpler, fewer merge conflicts)

## Issues Encountered
None

## Next Phase Readiness
- Complete CI/CD pipeline ready for validation (Plan 04-03)
- Apple Developer secrets needed for signing/notarization to activate

---
*Phase: 04-validation-and-distribution*
*Completed: 2026-02-25*
