# TONEWORD — Chat Summary

Original conversation: https://claude.ai/share/4a6d313b-e7b5-4c02-8ae8-306c78591146
Date: Feb 23, 2026
Participants: Jem + Claude

## How it started

Jem asked about innovating on EQ UX/control/feedback design for electric guitars — hardware pedals, VSTs, or digital pedals/modeler plugins.

Claude proposed several directions:
- **Spatial/gestural** — body-mapped EQ, touch-surface pedals, proximity control
- **Visual/synesthetic feedback** — spectral visualization on pedal, color-coded frequency feedback
- **Intelligence/context-awareness** — reference tone matching, adaptive EQ, room-aware EQ
- **Rethinking interaction** — semantic EQ, timeline/song-structure EQ, undo/history on hardware

## The "Semantic EQ" direction

Jem zeroed in on **Semantic EQ** — the idea that guitarists think in perceptual words ("glassy," "woody," "boxy") rather than frequency/gain/Q. Claude expanded on this:

- **Vocabulary as opportunity**: guitarists already have a rich shared lexicon that maps to complex multi-band curves
- **Multi-dimensional controls**: "Warm" isn't just "boost lows" — it's a combination of low-mid presence, gentle high rolloff, and saturation character
- **Three levels of vocabulary**: literal ("Bite"), perceptual ("Warmth"), poetic/synesthetic ("Velvet")
- **Teaching layer**: the tool should build frequency intuition over time, not hide complexity permanently — "the semantic layer is a bridge, not a wall"

## Key design decisions

- **Six tone dimensions chosen**: Warmth, Bite, Air, Body, Glass, Velvet — spanning literal to poetic
- **Bipolar sliders** (-100 to +100): each word works both ways (negative Warmth = "cold/thin")
- **11-band parametric EQ engine** under the hood, with cross-coupled gain mappings
- **Soft-clipping via `tanh`** prevents extreme gains when stacking dimensions
- **Negative cross-coupling**: boosting Glass automatically cuts Velvet frequencies (mirrors perceptual opposition)
- **"Under the hood" panel**: shows actual parametric EQ values to teach frequency intuition

## The "eyes vs. ears" thesis

Jem articulated the core motivation: **EQ UIs lead you to EQ with your eyes rather than your ears.** He moved from Pro-Q (very visual) back to a Neve channel strip emulation specifically because fewer visual cues meant he was mixing with his ears. TONEWORD inverts the traditional EQ paradigm — you describe tone in words, the tool translates to parameters, and you build frequency intuition over time rather than starting from it.

> "I just want to play around with that concept of maybe having an EQ with 16,000 individual frequencies that I possibly choose and adjustable cues and stuff like that, which gets in the way of me actually EQ-ing with my ears."

## Form factor decisions

- **Web Audio prototype first** — fast iteration, easy sharing
- **Two UI modes**: Studio (full sliders, spectrum, EQ curve) and Pedal (realistic hardware mockup with OLED, knobs, footswitch)
- **Pedal mode** modeled after Chase Bliss / Strymon aesthetic — brushed metal, corner screws, 3D perspective tilt
- Next: wrap in a **JUCE VST plugin** for DAW use
- Then: port to a **physical stompbox** using open-source pedal hardware

## Research findings (from conversation)

- **Polyend Endless** — launched at NAMM 2026. Open-source stompbox with C++ SDK and text-to-effect generator. $299, ARM Cortex-M7 at 720MHz. Generic rather than opinionated about interaction design.
- **Electro-Smith Daisy Seed** — recommended as the best path to physical hardware. bkshepherd has an open-source pedal design with OLED, 6 knobs, 2 footswitches, relay bypass — close to TONEWORD's pedal mode mockup.
- **MOD Devices Dwarf** — open-source, LV2 plugin format, but more expensive/complex platform.

## Files created in chat (contents NOT extractable from share link)

- `PITCH.md` — 3-paragraph pitch document with eyes-vs-ears thesis, Neve reference, ecosystem vision
- `TONEWORD-ROADMAP.md` — roadmap and research notes capturing next steps
- `semantic-eq.html` — the working prototype (downloaded locally as `toneword.html`)

## What was tried and didn't work

- First attempt to create the v2 (studio + pedal mode) HTML file got truncated mid-generation due to size. Claude had to rebuild it via bash to avoid truncation.
