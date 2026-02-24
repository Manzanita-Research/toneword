# TONEWORD

**Semantic EQ — speak your tone into being.**

TONEWORD replaces frequency/gain/Q knobs with perceptual tone words: Warmth, Bite, Air, Body, Glass, Velvet. Each word maps to a complex multi-band EQ curve under the hood, so you shape tone the way guitarists actually think about it — not the way audio engineers parameterize it.

## Why this exists

EQ interfaces are stuck in the audio engineering paradigm. Tools like Pro-Q give you surgical precision across thousands of frequency points, but they train you to EQ with your eyes instead of your ears. TONEWORD goes the other direction: describe what you want to hear, and build frequency intuition over time as the "under the hood" panel reveals what's actually happening.

## What it does

- **Six semantic dimensions** mapped to an 11-band parametric EQ engine
- **Bipolar controls** (-100 to +100) — negative Warmth is "cold/thin," negative Bite is "smooth/recessed"
- **Cross-coupled gain mappings** — boosting Glass automatically cuts Velvet frequencies, mirroring perceptual opposition
- **Soft-clipping via `tanh`** prevents extreme gains when stacking dimensions
- **Tone Recipes** — preset combinations (Nashville Twang, Midnight Jazz, Punk Scoop, etc.)
- **Two UI modes**: Studio (sliders + spectrum analyzer + EQ curve) and Pedal (realistic hardware mockup with OLED display and rotary knobs)

## Running it

Open `toneword.html` in a browser. Click "Mic / Guitar In" to process live input from an audio interface, or "Load Audio" to process a file. Toggle between Studio and Pedal views with the bar at the top.

## Tech

Single-file HTML/CSS/JS using the Web Audio API. No build system, no dependencies. The audio graph chains 11 `BiquadFilterNode` instances with gain values computed from the semantic state.

## Status

Working prototype. The semantic-to-parametric mapping is functional and sounds good. The pedal mode is a visual mockup — knobs drag, OLED renders, everything syncs with studio mode.

## Roadmap

1. **JUCE VST plugin** — bring TONEWORD into DAWs
2. **Physical stompbox** — Electro-Smith Daisy Seed platform, OLED + 6 encoders
3. **Broader ecosystem** — more "ears over eyes" tools for guitarists and producers
