#include "SliderPanel.h"
#include "TonewordLookAndFeel.h"
#include "../Parameters.h"

SliderPanel::SliderPanel (juce::AudioProcessorValueTreeState& apvts)
{
    // Grid order: Warmth/Bite (row 1), Air/Body (row 2), Glass/Velvet (row 3)
    struct SliderDef
    {
        const char* paramID;
        const char* name;
        const char* description;
        Dimension dim;
    };

    static constexpr SliderDef defs[] = {
        { ParamIDs::Warmth, "Warmth", "Low-end body and thickness",     Dimension::Warmth },
        { ParamIDs::Bite,   "Bite",   "Upper-mid presence and cut",     Dimension::Bite },
        { ParamIDs::Air,    "Air",    "Top-end shimmer and openness",   Dimension::Air },
        { ParamIDs::Body,   "Body",   "Full low-mid foundation",        Dimension::Body },
        { ParamIDs::Glass,  "Glass",  "Crystalline high-mid clarity",   Dimension::Glass },
        { ParamIDs::Velvet, "Velvet", "Smooth warmth and darkness",     Dimension::Velvet },
    };

    for (int i = 0; i < numSliders; ++i)
    {
        auto colour = TonewordLookAndFeel::getDimensionColour (defs[i].dim);
        sliders[static_cast<size_t> (i)] = std::make_unique<DimensionSlider> (
            apvts, defs[i].paramID, defs[i].name, defs[i].description, colour);
        addAndMakeVisible (*sliders[static_cast<size_t> (i)]);
    }
}

void SliderPanel::resized()
{
    auto bounds = getLocalBounds();
    constexpr int gap = 8;
    constexpr int cols = 2;
    constexpr int rows = 3;

    int cellWidth  = (bounds.getWidth() - gap * (cols - 1)) / cols;
    int cellHeight = (bounds.getHeight() - gap * (rows - 1)) / rows;

    for (int i = 0; i < numSliders; ++i)
    {
        int col = i % cols;
        int row = i / cols;
        int x = bounds.getX() + col * (cellWidth + gap);
        int y = bounds.getY() + row * (cellHeight + gap);
        sliders[static_cast<size_t> (i)]->setBounds (x, y, cellWidth, cellHeight);
    }
}
