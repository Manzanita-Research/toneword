# Phase 4: Validation and Distribution - Context

**Gathered:** 2026-02-24
**Status:** Ready for planning

<domain>
## Phase Boundary

The plugin passes format validation (pluginval, auval), is code-signed and notarized for macOS, and can be installed by someone who doesn't build it themselves. This phase does NOT add features — it makes the existing build shippable.

</domain>

<decisions>
## Implementation Decisions

### CI Validation Pipeline
- GitHub Actions workflow triggered on push to main and on PRs
- Run `pluginval` against both VST3 and AU builds
- Run `auval -v aufx Tone Mnzn` (or whatever the AU identifiers are) to validate the AU component
- macOS runner required (AU validation needs CoreAudio)
- Validation must pass before merge to main
- Build artifacts (VST3 + AU) uploaded as GitHub Actions artifacts for easy download

### Code Signing and Notarization
- Requires Apple Developer account (Developer ID Application certificate)
- Hardened runtime enabled with audio entitlements (`com.apple.security.device.audio-input`)
- Notarization via `notarytool` (modern Apple tooling, not altool)
- Signing and notarization run in CI after successful validation
- Secrets (certificate, Apple ID, team ID, app-specific password) stored as GitHub Actions secrets
- If no Apple Developer account exists yet: phase can initially validate without signing, then add signing as a follow-up step once credentials are available

### Distribution Packaging
- Single `.pkg` installer that installs both VST3 and AU components
- VST3 goes to `~/Library/Audio/Plug-Ins/VST3/`
- AU goes to `~/Library/Audio/Plug-Ins/Components/`
- Installer built with `pkgbuild` + `productbuild` (standard Apple tools)
- Installer is signed and notarized (same certificate)
- Staple the notarization ticket to the .pkg so it works offline
- No DMG wrapper needed — .pkg is cleaner for audio plugins

### Install Experience
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

</decisions>

<specifics>
## Specific Ideas

- User explicitly deferred all decisions to Claude — no specific preferences on tooling or workflow
- Priority: make it work simply and reliably. No over-engineering.
- Manzanita ethos: local-first, no surveillance — installer should not phone home or collect data

</specifics>

<deferred>
## Deferred Ideas

None — discussion stayed within phase scope

</deferred>

---

*Phase: 04-validation-and-distribution*
*Context gathered: 2026-02-24*
