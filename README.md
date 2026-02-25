# TONEWORD

**Semantic EQ — speak your tone into being.**

TONEWORD replaces frequency/gain/Q knobs with perceptual tone words: Warmth, Bite, Air, Body, Glass, Velvet. Each word maps to a complex multi-band EQ curve under the hood, so you shape tone the way guitarists actually think about it — not the way audio engineers parameterize it.

## Why this exists

EQ interfaces are stuck in the audio engineering paradigm. Tools like Pro-Q give you surgical precision across thousands of frequency points, but they train you to EQ with your eyes instead of your ears. TONEWORD goes the other direction: describe what you want to hear, and build frequency intuition over time as the teaching panel reveals what's actually happening.

## What it does

- **Six semantic dimensions** mapped to an 11-band parametric EQ engine
- **Bipolar controls** (-100 to +100) — negative Warmth is "cold/thin," negative Bite is "smooth/recessed"
- **Cross-coupled gain mappings** — boosting Glass automatically cuts Velvet frequencies, mirroring perceptual opposition
- **Soft-clipping via `tanh`** prevents extreme gains when stacking dimensions
- **Tone Recipes** — preset combinations (Nashville Twang, Midnight Jazz, Punk Scoop, etc.)
- **Teaching panel** — shows the EQ curve and per-band gains so you learn what "Warmth" actually means in frequency space

## Plugin

TONEWORD is a JUCE audio plugin (VST3, AU, Standalone) built on the [pamplejuce](https://github.com/sudara/pamplejuce) template. The DSP engine runs an 11-band parametric EQ with gains computed from six semantic dimensions.

Formats: **VST3** / **AU** / **Standalone**
Platform: **macOS 12+** (Universal Binary — Apple Silicon + Intel)

See [DEVELOPMENT.md](DEVELOPMENT.md) for build instructions.

## Web prototype

The `web/` directory contains the original browser-based prototype — a React + Vite app using the Web Audio API. It's where the semantic mapping was designed and tested before porting to JUCE.

## Status

v0.1.0. The plugin builds, passes pluginval (strictness 10) and auval validation, and has CI via GitHub Actions. The semantic-to-parametric mapping is functional and sounds good. GUI has studio mode with sliders, spectrum analyzer, EQ curve, and a teaching panel that breaks down per-band contributions.

## Roadmap

1. ~~JUCE plugin~~ — done
2. **Physical stompbox** — Electro-Smith Daisy Seed platform, OLED + 6 encoders
3. **Broader ecosystem** — more "ears over eyes" tools for guitarists and producers
