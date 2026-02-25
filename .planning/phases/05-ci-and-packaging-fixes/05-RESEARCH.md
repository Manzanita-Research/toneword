# Phase 5: CI and Packaging Fixes — Research

**Researched:** 2026-02-25
**Phase requirement IDs:** FMT-03

## Executive Summary

The v1.0 milestone audit identified CI workflow issues and a missing packaging resource. Research reveals that **one of the three reported issues was a false positive** — the GitHub Actions versions used are actually valid. The remaining issues (missing background.png, wrong .icns reference) are real and need fixing.

## Finding 1: GitHub Actions Versions Are Valid

The audit claimed `actions/checkout@v6`, `actions/upload-artifact@v6`, and `actions/download-artifact@v7` don't exist. **This is incorrect.**

- `actions/checkout@v6` — v6.0.2 released January 9, 2026 (latest stable)
- `actions/upload-artifact@v6` — released, uses Node.js 24
- `actions/download-artifact@v7` — v7.0.0 released December 12, 2025
- `mozilla-actions/sccache-action@v0.0.9` — already at latest version

**No action version changes needed.** The workflow uses current, valid versions.

The CONTEXT.md decision to "pin to @v4" was based on the audit's incorrect finding. We should keep the current versions since they're the latest stable releases.

## Finding 2: Missing background.png (REAL ISSUE)

`plugin/packaging/distribution.xml.template` references `background.png` on lines 9-10:
```xml
<background file="background.png" mime-type="image/png" scaling="tofit" alignment="bottomleft"/>
<background-darkAqua file="background.png" mime-type="image/png" scaling="tofit" alignment="bottomleft"/>
```

The `plugin/packaging/resources/` directory contains only:
- `EULA`
- `README`

No `background.png` exists. When `productbuild --resources ./resources` runs during packaging, it will fail to find this file.

**Fix options:**
1. Create a branded placeholder `background.png` in `plugin/packaging/resources/`
2. Remove background references from distribution.xml.template

Per CONTEXT.md decision: Create a branded placeholder image. Warm muted Manzanita tones (terracotta/sage/ochre). Dimensions should be appropriate for macOS pkg installer background (typically 620x418px).

**Implementation approach:** Generate a programmatic placeholder using ImageMagick or a simple script that creates a gradient/solid with brand colors. The image should feel intentional, not like a test image.

## Finding 3: pamplejuce.icns Reference (REAL ISSUE)

The "Add Custom Icons" step in `build_and_test.yml` (lines 126-132) references `packaging/pamplejuce.icns`:
```yaml
sips -i packaging/pamplejuce.icns
DeRez -only icns packaging/pamplejuce.icns > /tmp/icons
```

The file `plugin/packaging/pamplejuce.icns` exists but is the Pamplejuce template icon, not a Toneword-branded icon. Per CONTEXT.md: "Fix the `packaging/pamplejuce.icns` reference in 'Add Custom Icons' step to point to the correct Toneword .icns file (create placeholder if needed)."

**Fix:** Rename/replace the reference. Since we don't have a Toneword .icns yet, either:
- Rename pamplejuce.icns to toneword.icns and update the workflow reference
- Create a placeholder .icns from the existing icon.png in the packaging directory

## Finding 4: deploy.yml Is Fine

`deploy.yml` uses `actions/checkout@v4` which is valid. All other actions in deploy.yml are at current versions:
- `oven-sh/setup-bun@v2` — valid
- `actions/configure-pages@v5` — valid
- `actions/upload-pages-artifact@v3` — valid
- `actions/deploy-pages@v4` — valid

No changes needed to deploy.yml.

## Revised Scope

Given the research findings, Phase 5 scope is smaller than originally estimated:

1. **Create background.png** — branded placeholder for macOS pkg installer
2. **Fix .icns reference** — rename pamplejuce.icns to toneword.icns, update workflow
3. **No action version changes needed** — versions are already current and valid

The success criteria should be interpreted as:
1. "GitHub Actions workflow uses valid action versions" — ALREADY TRUE (v6/v7 are valid)
2. "`background.png` exists in packaging resources" — needs fix
3. "CI workflow can be pushed and run without immediate failures" — needs background.png + icns fix

## RESEARCH COMPLETE
