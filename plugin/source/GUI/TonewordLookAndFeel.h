#pragma once

#include "../DSP/BandConfig.h"
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * Custom LookAndFeel for the Toneword plugin.
 * Defines the Manzanita warm dark palette and custom slider styling.
 */
class TonewordLookAndFeel : public juce::LookAndFeel_V4
{
public:
    // ── Manzanita palette ──────────────────────────────────────────────
    static constexpr juce::uint32 colBg       = 0xFF1A1A1A; // warm dark, not pure black
    static constexpr juce::uint32 colSurface  = 0xFF242422;
    static constexpr juce::uint32 colSurface2 = 0xFF2C2A27;
    static constexpr juce::uint32 colBorder   = 0xFF3A3835;
    static constexpr juce::uint32 colText     = 0xFFE8E4DF;
    static constexpr juce::uint32 colTextDim  = 0xFF78756F;
    static constexpr juce::uint32 colTextMid  = 0xFFA8A49E;

    // ── Dimension colors (indexed by Dimension enum) ───────────────────
    static constexpr std::array<juce::uint32, NUM_DIMENSIONS> dimensionColours = {{
        0xFFC4723A, // Warmth — terracotta/orange
        0xFFB85A5A, // Bite   — coral/pink
        0xFF5A8AB8, // Air    — sky blue
        0xFFB89A5A, // Body   — golden/ochre
        0xFF5AB8A8, // Glass  — teal/green
        0xFF8A5AB8  // Velvet — lavender/purple
    }};

    /** Get the colour for a given dimension. */
    static juce::Colour getDimensionColour (Dimension d)
    {
        return juce::Colour (dimensionColours[static_cast<size_t> (d)]);
    }

    TonewordLookAndFeel();

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           juce::Slider::SliderStyle style, juce::Slider& slider) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TonewordLookAndFeel)
};
