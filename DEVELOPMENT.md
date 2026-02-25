# Development

## Prerequisites

- CMake 3.25+
- Ninja (recommended) or Xcode
- macOS 12+ SDK
- Bun (for the web prototype)

## Building the plugin

```bash
cd plugin
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Build outputs land in `plugin/build/Toneword_artefacts/Release/`:
- `Standalone/Toneword.app`
- `VST3/Toneword.vst3`
- `AU/Toneword.component`

To install the AU for local testing:

```bash
sudo cp -R build/Toneword_artefacts/Release/AU/Toneword.component /Library/Audio/Plug-Ins/Components/
```

## Running tests

```bash
cd plugin
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --target TonewordTests
./build/tests/TonewordTests
```

Tests use Catch2 and cover filter chain behavior, gain calculations, preset loading, and basic plugin lifecycle.

## Project structure

```
plugin/
  source/
    DSP/              # SemanticEQ engine, band config
    GUI/              # JUCE components — sliders, spectrum, EQ curve, teaching panel
    Presets/          # Tone recipe .xml files
    PluginProcessor   # JUCE audio processor
    PluginEditor      # JUCE editor (GUI root)
    Parameters.h      # APVTS parameter layout
  tests/              # Catch2 test suite
  packaging/          # Installer resources (icon, EULA, distribution template)
  cmake/              # Pamplejuce CMake helpers
  JUCE/               # JUCE framework (git submodule)
web/                  # Browser prototype (React + Vite + Web Audio API)
```

## CI

GitHub Actions runs on push to main and on PRs. The workflow builds, runs Catch2 tests, validates with pluginval (strictness 10) and auval, and uploads build artifacts. On version tags (`v*`), it creates a GitHub Release with a signed `.pkg` installer (when Apple Developer secrets are configured).

## Web prototype

```bash
cd web
bun install
bun dev
```

The web version is the original design prototype. It's not actively developed but useful as a reference for the semantic mapping logic.
