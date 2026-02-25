#include "SpectrumPanel.h"

// Manzanita palette constants (warm dark, not pure black)
namespace Palette
{
    static constexpr juce::uint32 surface   = 0xFF242422;
    static constexpr juce::uint32 border    = 0xFF3A3835;
    static constexpr juce::uint32 textDim   = 0xFF78756F;
    static constexpr juce::uint32 specFill  = 0xFFC4723A; // terracotta (warmth color)
}

SpectrumPanel::SpectrumPanel (SpectrumData& data)
    : spectrumData (data)
{
    startTimerHz (30);
}

SpectrumPanel::~SpectrumPanel()
{
    stopTimer();
}

void SpectrumPanel::timerCallback()
{
    if (spectrumData.isNextBlockReady())
    {
        spectrumData.processBlock();
        repaint();
    }
}

void SpectrumPanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float w = bounds.getWidth();
    float h = bounds.getHeight();

    // Dark background
    g.setColour (juce::Colour (Palette::surface));
    g.fillRect (bounds);

    // Reserve space for axis labels at bottom
    constexpr float labelHeight = 16.0f;
    float plotH = h - labelHeight;

    // Draw frequency axis labels: 100, 1k, 10k
    g.setColour (juce::Colour (Palette::textDim));
    g.setFont (juce::FontOptions (10.0f));

    struct FreqLabel { float freq; const char* text; };
    constexpr FreqLabel labels[] = {
        { 100.0f, "100" },
        { 1000.0f, "1k" },
        { 10000.0f, "10k" }
    };

    for (const auto& label : labels)
    {
        float x = GUIHelpers::freqToX (label.freq, w);
        // Thin vertical grid line
        g.setColour (juce::Colour (Palette::border));
        g.drawLine (x, 0.0f, x, plotH, 0.5f);
        // Label text below
        g.setColour (juce::Colour (Palette::textDim));
        g.drawText (label.text, juce::Rectangle<float> (x - 15.0f, plotH + 1.0f, 30.0f, labelHeight - 1.0f),
                    juce::Justification::centred, false);
    }

    // Draw spectrum as a filled path
    const auto& scope = spectrumData.getScopeData();

    juce::Path spectrumPath;
    bool pathStarted = false;

    for (int i = 0; i < SpectrumData::scopeSize; ++i)
    {
        // Map scope index to frequency (20Hz to 20kHz, log-skewed like processBlock)
        float proportion = (float) i / (float) SpectrumData::scopeSize;
        float freq = 20.0f * std::pow (1000.0f, proportion);
        float x = GUIHelpers::freqToX (freq, w);
        float y = juce::jmap (scope[static_cast<size_t> (i)], 0.0f, 1.0f, plotH, 0.0f);

        if (! pathStarted)
        {
            spectrumPath.startNewSubPath (x, y);
            pathStarted = true;
        }
        else
        {
            spectrumPath.lineTo (x, y);
        }
    }

    // Close the path along the bottom for fill
    if (pathStarted)
    {
        spectrumPath.lineTo (GUIHelpers::freqToX (20000.0f, w), plotH);
        spectrumPath.lineTo (GUIHelpers::freqToX (20.0f, w), plotH);
        spectrumPath.closeSubPath();

        // Gradient fill: terracotta at bottom, transparent at top
        auto fillColor = juce::Colour (Palette::specFill);
        g.setGradientFill (juce::ColourGradient (
            fillColor.withAlpha (0.6f), 0.0f, plotH,
            fillColor.withAlpha (0.05f), 0.0f, 0.0f,
            false));
        g.fillPath (spectrumPath);

        // Stroke the top edge
        g.setColour (fillColor.withAlpha (0.8f));
        // Re-create just the top edge for stroking
        juce::Path strokePath;
        bool strokeStarted = false;
        for (int i = 0; i < SpectrumData::scopeSize; ++i)
        {
            float proportion = (float) i / (float) SpectrumData::scopeSize;
            float freq = 20.0f * std::pow (1000.0f, proportion);
            float x = GUIHelpers::freqToX (freq, w);
            float y = juce::jmap (scope[static_cast<size_t> (i)], 0.0f, 1.0f, plotH, 0.0f);

            if (! strokeStarted)
            {
                strokePath.startNewSubPath (x, y);
                strokeStarted = true;
            }
            else
            {
                strokePath.lineTo (x, y);
            }
        }
        g.strokePath (strokePath, juce::PathStrokeType (1.5f));
    }
}
