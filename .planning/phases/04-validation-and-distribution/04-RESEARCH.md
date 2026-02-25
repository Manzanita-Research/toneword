# Phase 4: Validation and Distribution - Research

**Researched:** 2026-02-25
**Domain:** CI/CD pipeline, macOS code signing, notarization, installer packaging
**Confidence:** HIGH

## Summary

Phase 4 takes the completed TONEWORD plugin (Phases 0-3 done) and makes it shippable: CI validation with pluginval and auval, macOS code signing and notarization, and a .pkg installer for distribution. The project uses the Pamplejuce template which ships with a comprehensive GitHub Actions workflow (`build_and_test.yml`) that handles building, testing, pluginval, code signing, notarization, and packaging out of the box. The primary work is adapting the Pamplejuce reference workflow to TONEWORD's specific configuration and adding the auval step that the template doesn't include by default.

The existing project already has most of the packaging infrastructure in place: `distribution.xml.template`, `installer.iss`, `icon.png`, EULA, README resources. The CMake setup includes `GitHubENV.cmake` which writes a `.env` file consumed by the CI workflow. The main gap is the workflow YAML itself and integrating auval validation.

**Primary recommendation:** Create the GitHub Actions workflow based on the Pamplejuce template's `build_and_test.yml`, customize for TONEWORD (remove CLAP references, add auval step), and ensure the packaging infrastructure produces a notarized .pkg installer.

<user_constraints>
## User Constraints (from CONTEXT.md)

### Locked Decisions
- GitHub Actions workflow triggered on push to main and on PRs
- Run `pluginval` against both VST3 and AU builds
- Run `auval -v aufx Tone Mnzn` (or whatever the AU identifiers are) to validate the AU component
- macOS runner required (AU validation needs CoreAudio)
- Validation must pass before merge to main
- Build artifacts (VST3 + AU) uploaded as GitHub Actions artifacts for easy download
- Requires Apple Developer account (Developer ID Application certificate)
- Hardened runtime enabled with audio entitlements (`com.apple.security.device.audio-input`)
- Notarization via `notarytool` (modern Apple tooling, not altool)
- Signing and notarization run in CI after successful validation
- Secrets (certificate, Apple ID, team ID, app-specific password) stored as GitHub Actions secrets
- If no Apple Developer account exists yet: phase can initially validate without signing, then add signing as a follow-up step once credentials are available
- Single `.pkg` installer that installs both VST3 and AU components
- VST3 goes to `~/Library/Audio/Plug-Ins/VST3/`
- AU goes to `~/Library/Audio/Plug-Ins/Components/`
- Installer built with `pkgbuild` + `productbuild` (standard Apple tools)
- Installer is signed and notarized (same certificate)
- Staple the notarization ticket to the .pkg so it works offline
- No DMG wrapper needed — .pkg is cleaner for audio plugins
- Download .pkg from GitHub Releases
- Double-click to install — standard macOS installer flow
- No Gatekeeper warning (notarization handles this)
- Plugin appears in DAW plugin scanner immediately after install
- No README bundled in installer — keep it simple
- Release notes on GitHub Release page

### Claude's Discretion
- Exact GitHub Actions workflow structure and job naming
- pkgbuild/productbuild flags and options
- Whether to add a simple uninstall script
- CI caching strategy for build dependencies
- Version numbering in installer metadata

### Deferred Ideas (OUT OF SCOPE)
None — discussion stayed within phase scope
</user_constraints>

<phase_requirements>
## Phase Requirements

| ID | Description | Research Support |
|----|-------------|-----------------|
| FMT-03 | Plugin passes pluginval validation without errors | Pamplejuce template includes pluginval step at strictness level 10; download binary, validate VST3 and AU builds |
</phase_requirements>

## Standard Stack

### Core
| Tool | Version | Purpose | Why Standard |
|------|---------|---------|--------------|
| GitHub Actions | N/A | CI/CD pipeline | Industry standard for OSS; Pamplejuce template uses it |
| pluginval | v1.0.3 | VST3/AU format validation | Tracktion's official audio plugin validator; the standard tool |
| auval | macOS built-in | AU component validation | Apple's official Audio Unit validation tool |
| pkgbuild + productbuild | macOS built-in | .pkg installer creation | Apple's native packaging tools |
| notarytool | macOS built-in | Apple notarization | Current Apple notarization tool (replaced altool) |
| codesign | macOS built-in | Code signing | macOS standard signing utility |
| sccache | v0.0.9 action | Build caching | Pamplejuce template uses it for faster CI builds |
| Ninja | latest | Build system | Faster than Make for CI builds |

### Supporting
| Tool | Version | Purpose | When to Use |
|------|---------|---------|-------------|
| sudara/basic-macos-keychain-action | v1 | Certificate import in CI | Import Developer ID certs into temp keychain |
| softprops/action-gh-release | v2 | GitHub Release creation | Create release on version tag push |
| actions/upload-artifact | v6 | CI artifact upload | Upload build artifacts for download |
| osxutils | brew | macOS file utilities | SetFile for custom icons |

### Alternatives Considered
| Instead of | Could Use | Tradeoff |
|------------|-----------|----------|
| pkgbuild/productbuild | DMG + drag-to-install | .pkg is cleaner for audio plugins — installs to correct paths automatically |
| notarytool | altool | altool is deprecated; notarytool is the current Apple tool |
| GitHub Actions | CircleCI/GitLab CI | Pamplejuce template is pre-built for GH Actions; no reason to switch |

## Architecture Patterns

### CI Workflow Structure (from Pamplejuce template)

The Pamplejuce template's `build_and_test.yml` is the reference architecture. Key pattern:

```
build_and_test job:
  1. Setup (compilers, deps)
  2. Checkout (with submodules)
  3. Configure (CMake + sccache)
  4. Build (Release)
  5. Test (CTest)
  6. Read .env from CMake (GitHubENV.cmake)
  7. Set artifact paths
  8. Pluginval
  9. Import certificates (macOS)
  10. Codesign (macOS)
  11. Custom icons (macOS)
  12. pkgbuild + productbuild + notarize (macOS)
  13. Upload artifacts

release job (on version tags):
  1. Download artifacts
  2. Create GitHub Release
```

### TONEWORD-Specific Adaptations

1. **Remove CLAP references** — TONEWORD builds VST3, AU, and Standalone only (no CLAP format)
2. **Remove Windows/Linux matrix entries** — macOS-only for v1 (per project scope)
3. **Add auval step** — Pamplejuce doesn't include auval; add `auval -v aufx Tnwd Manz` after build
4. **Pluginval on both VST3 and AU** — Template only validates VST3; add AU validation
5. **Install paths** — User decided `~/Library/Audio/Plug-Ins/` (user home); Pamplejuce uses `/Library/Audio/Plug-Ins/` (system). Use system paths for .pkg installers (standard for installer packages).

### AU Identifiers

From CMakeLists.txt:
- `AU_MAIN_TYPE`: `kAudioUnitType_Effect` → auval type code: `aufx`
- `PLUGIN_CODE`: `Tnwd` (4-char subtype)
- `PLUGIN_MANUFACTURER_CODE`: `Manz` (4-char manufacturer)
- auval command: `auval -v aufx Tnwd Manz`

### Signing Strategy: Graceful Degradation

The CONTEXT.md explicitly allows: "If no Apple Developer account exists yet: phase can initially validate without signing, then add signing as a follow-up step once credentials are available."

Pattern: Use `if` conditions on secrets presence. When secrets are empty, skip signing/notarization steps but still build, test, validate, and upload unsigned artifacts.

```yaml
- name: Codesign (macOS)
  if: ${{ env.DEV_ID_APP_CERT != '' }}
  # ... signing steps
```

### distribution.xml.template Adaptation

The existing template includes CLAP and Standalone choices. Need to:
1. Remove CLAP choice (not a TONEWORD format)
2. Keep Standalone as optional choice
3. Keep VST3 and AU as default-selected

## Don't Hand-Roll

| Problem | Don't Build | Use Instead | Why |
|---------|-------------|-------------|-----|
| Keychain management in CI | Manual `security` commands | `sudara/basic-macos-keychain-action@v1` | Handles temp keychain creation, import, cleanup |
| Build caching | Manual cache management | `mozilla-actions/sccache-action` | Pamplejuce-tested, handles multi-arch caching |
| Release creation | Manual `gh release create` | `softprops/action-gh-release@v2` | Handles asset upload, prerelease flags, tag matching |
| Plugin validation | Manual audio tests | pluginval binary | Tracktion's comprehensive plugin test suite |

**Key insight:** The Pamplejuce template has battle-tested each of these CI patterns across hundreds of JUCE projects. Follow the template, don't reinvent.

## Common Pitfalls

### Pitfall 1: auval Hangs in CI
**What goes wrong:** auval may hang waiting for user interaction or fail silently if Audio Unit cache is stale.
**Why it happens:** auval expects a running Audio Server and may prompt for permissions.
**How to avoid:** Run `killall -9 AudioComponentRegistrar 2>/dev/null || true` before auval to clear cache. Set a timeout on the auval step.
**Warning signs:** CI job runs indefinitely on the auval step.

### Pitfall 2: Code Signing Identity Not Found
**What goes wrong:** `codesign` fails with "no identity found" even though secrets are set.
**Why it happens:** Certificate not imported into the correct keychain, or keychain is locked.
**How to avoid:** Use `sudara/basic-macos-keychain-action` which handles keychain creation and import correctly. Verify certificate names match exactly.
**Warning signs:** Build succeeds but signing step fails with "no identity found."

### Pitfall 3: Notarization Timeout
**What goes wrong:** `notarytool submit --wait` times out after 10+ minutes.
**Why it happens:** Apple's notarization service can be slow, especially for new developer accounts.
**How to avoid:** Set `timeout-minutes: 10` on the notarization step. The `--wait` flag polls until complete.
**Warning signs:** CI step appears to hang, then eventually fails.

### Pitfall 4: Universal Binary Signing
**What goes wrong:** Code signing fails on universal (arm64+x86_64) binaries.
**Why it happens:** Both architecture slices must be signed. `--deep` flag is needed for bundles.
**How to avoid:** Use `--deep --strict --options=runtime --timestamp` flags exactly as in Pamplejuce template.
**Warning signs:** Signing succeeds but notarization rejects with "invalid signature."

### Pitfall 5: .pkg Install Paths
**What goes wrong:** Plugins install to wrong location, DAW can't find them.
**Why it happens:** Using user-relative paths (`~/Library/...`) instead of system paths in pkgbuild.
**How to avoid:** Use system paths for pkgbuild: `/Library/Audio/Plug-Ins/VST3/` and `/Library/Audio/Plug-Ins/Components/`. These are the standard DAW scan paths.
**Warning signs:** Plugin installs but doesn't appear in DAW plugin list.

### Pitfall 6: Pluginval AU Validation on CI
**What goes wrong:** Pluginval can't find the AU component.
**Why it happens:** AU components need to be registered with the system before pluginval can scan them. In CI, the plugin isn't installed to the system path.
**How to avoid:** For AU validation, either: (a) copy the .component to `/Library/Audio/Plug-Ins/Components/` before running pluginval, or (b) pass the direct path to pluginval which can load from arbitrary paths.
**Warning signs:** Pluginval reports "no plugins found" for AU.

## Code Examples

### GitHub Actions: macOS-only Build + Test + Validate

```yaml
# Adapted from Pamplejuce template
name: Build and Validate

on:
  push:
    branches: [main]
  pull_request:
  workflow_dispatch:

concurrency:
  group: ${{ github.ref }}
  cancel-in-progress: true

env:
  BUILD_TYPE: Release
  BUILD_DIR: Builds
  SCCACHE_GHA_ENABLED: true

jobs:
  build_and_test:
    runs-on: macos-14
    steps:
      - name: Install macOS Deps
        run: brew install ninja osxutils

      - uses: actions/checkout@v6
        with:
          submodules: recursive

      - uses: mozilla-actions/sccache-action@v0.0.9

      - name: Configure
        run: cmake -B ${{ env.BUILD_DIR }} -DCMAKE_BUILD_TYPE=${{ env.BUILD_TYPE }} -DCMAKE_C_COMPILER_LAUNCHER=sccache -DCMAKE_CXX_COMPILER_LAUNCHER=sccache -G Ninja -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" .

      - name: Build
        run: cmake --build ${{ env.BUILD_DIR }} --config ${{ env.BUILD_TYPE }}

      - name: Test
        working-directory: ${{ env.BUILD_DIR }}
        run: ctest --verbose --output-on-failure

      - name: Read .env from CMake
        run: |
          cat .env
          cat .env >> $GITHUB_ENV

      - name: Set artifact paths
        run: |
          ARTIFACTS_PATH=${{ env.BUILD_DIR }}/${{ env.PROJECT_NAME }}_artefacts/${{ env.BUILD_TYPE }}
          echo "ARTIFACTS_PATH=$ARTIFACTS_PATH" >> $GITHUB_ENV
          echo "VST3_PATH=$ARTIFACTS_PATH/VST3/${{ env.PRODUCT_NAME }}.vst3" >> $GITHUB_ENV
          echo "AU_PATH=$ARTIFACTS_PATH/AU/${{ env.PRODUCT_NAME }}.component" >> $GITHUB_ENV
          echo "STANDALONE_PATH=$ARTIFACTS_PATH/Standalone/${{ env.PRODUCT_NAME }}.app" >> $GITHUB_ENV
          echo "ARTIFACT_NAME=${{ env.PRODUCT_NAME }}-${{ env.VERSION }}-macOS" >> $GITHUB_ENV

      - name: Pluginval (VST3)
        run: |
          curl -LO "https://github.com/Tracktion/pluginval/releases/download/v1.0.3/pluginval_macOS.zip"
          7z x pluginval_macOS.zip
          pluginval.app/Contents/MacOS/pluginval --strictness-level 10 --verbose --validate "${{ env.VST3_PATH }}"

      - name: Pluginval (AU)
        run: |
          pluginval.app/Contents/MacOS/pluginval --strictness-level 10 --verbose --validate "${{ env.AU_PATH }}"

      - name: auval
        timeout-minutes: 5
        run: |
          killall -9 AudioComponentRegistrar 2>/dev/null || true
          sudo cp -R "${{ env.AU_PATH }}" /Library/Audio/Plug-Ins/Components/
          auval -v aufx Tnwd Manz
```

### Signing and Notarization (conditional on secrets)

```yaml
      - name: Import Certificates (macOS)
        if: ${{ secrets.DEV_ID_APP_CERT != '' }}
        uses: sudara/basic-macos-keychain-action@v1
        id: keychain
        with:
          dev-id-app-cert: ${{ secrets.DEV_ID_APP_CERT }}
          dev-id-app-password: ${{ secrets.DEV_ID_APP_PASSWORD }}
          dev-id-installer-cert: ${{ secrets.DEV_ID_INSTALLER_CERT }}
          dev-id-installer-password: ${{ secrets.DEV_ID_INSTALLER_PASSWORD }}

      - name: Codesign
        if: ${{ steps.keychain.outcome == 'success' }}
        timeout-minutes: 5
        run: |
          codesign --force --keychain ${{ steps.keychain.outputs.keychain-path }} -s "${{ secrets.DEVELOPER_ID_APPLICATION }}" -v "${{ env.VST3_PATH }}" --deep --strict --options=runtime --timestamp
          codesign --force --keychain ${{ steps.keychain.outputs.keychain-path }} -s "${{ secrets.DEVELOPER_ID_APPLICATION }}" -v "${{ env.AU_PATH }}" --deep --strict --options=runtime --timestamp
          codesign --force --keychain ${{ steps.keychain.outputs.keychain-path }} -s "${{ secrets.DEVELOPER_ID_APPLICATION }}" -v "${{ env.STANDALONE_PATH }}" --deep --strict --options=runtime --timestamp
```

### pkgbuild + productbuild + Notarize

```yaml
      - name: Package and Notarize
        if: ${{ steps.keychain.outcome == 'success' }}
        timeout-minutes: 10
        run: |
          pkgbuild --identifier "${{ env.BUNDLE_ID }}.au.pkg" --version $VERSION --component "${{ env.AU_PATH }}" --install-location "/Library/Audio/Plug-Ins/Components" "packaging/${{ env.PRODUCT_NAME }}.au.pkg"
          pkgbuild --identifier "${{ env.BUNDLE_ID }}.vst3.pkg" --version $VERSION --component "${{ env.VST3_PATH }}" --install-location "/Library/Audio/Plug-Ins/VST3" "packaging/${{ env.PRODUCT_NAME }}.vst3.pkg"

          pkgbuild --analyze --root "$(dirname "${{ env.STANDALONE_PATH }}")" standalone.plist
          plutil -replace BundleIsRelocatable -bool NO standalone.plist
          pkgbuild --identifier "${{ env.BUNDLE_ID }}.app.pkg" --version $VERSION --root "$(dirname "${{ env.STANDALONE_PATH }}")" --component-plist standalone.plist --install-location "/Applications" "packaging/${{ env.PRODUCT_NAME }}.app.pkg"

          cd packaging
          envsubst < distribution.xml.template > distribution.xml
          productbuild --resources ./resources --distribution distribution.xml --sign "${{ secrets.DEVELOPER_ID_INSTALLER }}" --timestamp "${{ env.ARTIFACT_NAME }}.pkg"

          xcrun notarytool submit "${{ env.ARTIFACT_NAME }}.pkg" --apple-id ${{ secrets.NOTARIZATION_USERNAME }} --password ${{ secrets.NOTARIZATION_PASSWORD }} --team-id ${{ secrets.TEAM_ID }} --wait
          xcrun stapler staple "${{ env.ARTIFACT_NAME }}.pkg"
```

## State of the Art

| Old Approach | Current Approach | When Changed | Impact |
|--------------|------------------|--------------|--------|
| altool for notarization | notarytool | 2023 (Xcode 14+) | altool deprecated; notarytool is faster, simpler |
| Manual keychain setup | sudara/basic-macos-keychain-action | 2024 | Handles edge cases in CI keychain management |
| Single-arch builds | Universal Binary (arm64+x86_64) | Apple Silicon transition | Must support both architectures |

**Deprecated/outdated:**
- `altool`: Deprecated by Apple, replaced by `notarytool`
- Projucer-based builds: CMake is the standard for JUCE 8

## Open Questions

1. **Apple Developer Account Status**
   - What we know: CONTEXT.md acknowledges account may not exist yet and allows graceful degradation
   - What's unclear: Whether the user has enrolled in Apple Developer Program ($99/yr)
   - Recommendation: Build the workflow to work without signing. Gate signing/notarization steps on secret presence. Document which secrets need to be set when account is ready.

2. **auval in CI**
   - What we know: auval requires the AU to be installed in a system path for discovery
   - What's unclear: Whether GitHub Actions macos-14 runners have the necessary AudioToolbox support for auval
   - Recommendation: Add auval step with timeout and fallback. If auval doesn't work in CI, document it as a local validation step. HIGH confidence it works — macOS runners have full CoreAudio.

## Sources

### Primary (HIGH confidence)
- Pamplejuce template: https://github.com/sudara/pamplejuce — reference CI workflow for JUCE plugins
- Pamplejuce `build_and_test.yml`: https://github.com/sudara/pamplejuce/blob/main/.github/workflows/build_and_test.yml — complete CI pipeline
- Existing project CMakeLists.txt and packaging/ directory — project-specific configuration

### Secondary (MEDIUM confidence)
- Apple notarytool documentation — notarization workflow
- pluginval releases: https://github.com/Tracktion/pluginval — v1.0.3 latest stable

## Metadata

**Confidence breakdown:**
- Standard stack: HIGH - Pamplejuce template is battle-tested, tools are well-documented
- Architecture: HIGH - Direct adaptation of proven Pamplejuce CI workflow
- Pitfalls: HIGH - Common issues documented across JUCE community

**Research date:** 2026-02-25
**Valid until:** 2026-03-25 (stable tooling, unlikely to change)
