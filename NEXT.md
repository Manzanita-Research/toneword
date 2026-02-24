# TONEWORD — Next Steps

## Phase 1: JUCE VST Plugin
- [ ] Port the 11-band parametric EQ engine to JUCE's `IIRFilter` / `dsp::ProcessorChain`
- [ ] Implement the semantic-to-parametric mapping in C++
- [ ] Build a plugin UI (could start minimal — sliders + curve visualization)
- [ ] Validate latency and CPU usage in a DAW context
- [ ] Package as VST3/AU for macOS (and eventually Windows)

## Phase 2: Physical Stompbox
- [ ] Research Electro-Smith Daisy Seed pedal hardware (bkshepherd's open-source design is a strong starting point — OLED + 6 knobs + 2 footswitches + relay bypass)
- [ ] Port DSP from JUCE/Web Audio to Daisy's C++ DSP library
- [ ] Design OLED UI (miniaturized version of pedal mode canvas)
- [ ] Prototype on Daisy Seed dev board
- [ ] PCB design for production enclosure (125B form factor — 6 knobs is upper bound, may need page/bank system)

## Phase 3: Ecosystem
- [ ] Explore what other "ears over eyes" tools look like for producers (compression? reverb? mixing?)
- [ ] Conversational/text-prompt tone shaping ("more aggressive but not harsh")
- [ ] Custom vocabulary — personal tone languages with user-defined mappings
- [ ] A/B snapshot history for tonal undo/compare
- [ ] Community tone recipe sharing

## Competitive landscape
- **Polyend Endless** — open-source, text-to-effect, $299. Generic platform vs. TONEWORD's opinionated interaction design.
- **Neural DSP / ToneDex** — tone matching but not semantic control
- **Izotope Neutron** — has semantic-ish controls but aimed at mixing engineers, not guitarists in real-time
