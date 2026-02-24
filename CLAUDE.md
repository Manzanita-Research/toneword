# CLAUDE.md — TONEWORD

## What this is

TONEWORD is a semantic EQ for guitarists. Instead of frequency/gain/Q controls, it uses perceptual tone words (Warmth, Bite, Air, Body, Glass, Velvet) that map to complex multi-band parametric EQ curves. The core thesis: EQ UIs train you to mix with your eyes instead of your ears. TONEWORD inverts that — you describe tone in words, and the tool teaches you frequency over time.

## Architecture

Single-file HTML prototype (`toneword.html`) using the Web Audio API. No framework, no build system.

**Audio engine:**
- 11 `BiquadFilterNode` instances chained in series (lowshelf → 9x peaking → highshelf + one more peaking)
- Each band has a `gainMap` defining how much each semantic dimension affects it (with both positive and negative weights for cross-coupling)
- Gain calculation: sum weighted contributions from all dimensions, then soft-clip with `tanh` to prevent extreme values
- Max gain: ±12dB per band

**Two UI modes** sharing a single audio engine:
- **Studio mode**: full-width layout with range sliders, real-time spectrum analyzer (FFT), composite EQ curve visualization, and a "what's happening under the hood" param table
- **Pedal mode**: realistic hardware mockup (Chase Bliss / Strymon aesthetic) with draggable rotary knobs, OLED canvas display, footswitch bypass, and preset navigation

**State management**: a single `semanticState` object (`{warmth, bite, air, body, glass, velvet}` each -100 to +100) is the source of truth. Both UI modes read from and write to it. `applyState()` propagates changes to the audio engine and both UIs.

## Key design decisions (settled)

- **Six dimensions, not more**: Warmth, Bite, Air, Body, Glass, Velvet — spanning literal to poetic. This set was carefully chosen to cover the tonal space without redundancy.
- **Bipolar sliders**: -100 to +100 so each word works in both directions.
- **Cross-coupling with negative weights**: boosting Glass cuts Velvet frequencies and vice versa. This mirrors how these qualities actually oppose each other perceptually.
- **`tanh` soft-clipping**: prevents stacking multiple dimensions from producing extreme gains. This is important and should be preserved in any port.
- **Teaching layer**: the "under the hood" panel shows actual parametric EQ values. The semantic layer is a bridge, not a wall — users should build frequency intuition over time.

## What's working

- Full audio processing pipeline (mic input, file loading, 11-band EQ, spectrum analysis)
- All six semantic dimensions with correct gain mappings
- Eight tone recipe presets
- Studio mode with sliders, real-time spectrum, EQ curve, parameter table
- Pedal mode with knobs, OLED display, footswitch, preset navigation
- Bidirectional sync between studio and pedal views
- Bypass functionality

## What's not built yet

- **JUCE VST plugin port** — next major milestone. Would validate the concept with the broadest audience (DAW users)
- **Physical stompbox** — Electro-Smith Daisy Seed is the target platform. bkshepherd's open-source pedal design (OLED + 6 knobs + 2 footswitches) is close to our pedal mode mockup
- **Conversational/text-prompt interface** — "more aggressive but not harsh" style natural language tone shaping
- **Custom vocabulary** — let users define their own tone words with personal mappings
- **A/B snapshot history** — tonal undo/compare
- **PITCH.md** — was created in the original chat session but the content wasn't extractable from the share link. Needs to be rewritten. Core thesis: EQ UI is broken (eyes over ears), TONEWORD fixes it, first piece of a broader ecosystem.

## Fonts

- Display: Instrument Serif (italic for "WORD" in logo)
- Mono/UI: DM Mono
- Both loaded from Google Fonts

## Color palette

Dark theme with warm accents:
- Background: `#0a0a0b`, surfaces: `#131315` / `#1a1a1e`
- Each dimension has a distinct accent color: Warmth `#c4723a`, Bite `#b85a5a`, Air `#5a8ab8`, Body `#b89a5a`, Glass `#5ab8a8`, Velvet `#8a5ab8`

## Hardware research notes

- **Polyend Endless**: open-source stompbox with C++ SDK and text-to-effect generator. $299, ARM Cortex-M7 at 720MHz. Competitor/inspiration but generic rather than opinionated.
- **Electro-Smith Daisy Seed**: best path to physical pedal. STM32H750 at 480MHz, 64MB SDRAM, codec built in. Daisy's C++ DSP library maps fairly directly from Web Audio BiquadFilters.
- **MOD Devices Dwarf**: open-source, LV2 plugin format. More complex platform, higher price point.
