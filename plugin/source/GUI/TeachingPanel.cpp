#include "TeachingPanel.h"
#include "TonewordLookAndFeel.h"

TeachingPanel::TeachingPanel (juce::AudioProcessorValueTreeState& apvts)
    : apvtsRef (apvts)
{
    // Cache atomic parameter pointers for lock-free GUI-thread reads
    for (int i = 0; i < NUM_DIMENSIONS; ++i)
        paramPtrs[static_cast<size_t> (i)] = apvtsRef.getRawParameterValue (ParamIDs::allIDs[i]);

    toggleButton.setButtonText ("What's happening under the hood");
    toggleButton.onClick = [this]()
    {
        expanded = ! expanded;

        if (expanded)
            startTimerHz (10);
        else
            stopTimer();

        // Ask parent to recalculate layout so our height changes
        if (auto* parent = getParentComponent())
            parent->resized();
    };

    addAndMakeVisible (toggleButton);

    cachedGains.fill (0.0f);
}

TeachingPanel::~TeachingPanel()
{
    stopTimer();
}

int TeachingPanel::getPreferredHeight() const
{
    if (! expanded)
        return 36; // just the button

    // button (36) + header row (22) + 11 data rows (22 each) + 8 padding
    return 36 + 22 + (NUM_BANDS * 22) + 8;
}

void TeachingPanel::resized()
{
    auto area = getLocalBounds();
    toggleButton.setBounds (area.removeFromTop (36));
}

void TeachingPanel::timerCallback()
{
    if (! expanded)
        return;

    // Read current dimension values from APVTS
    std::array<float, NUM_DIMENSIONS> dims {};
    for (int i = 0; i < NUM_DIMENSIONS; ++i)
        dims[static_cast<size_t> (i)] = paramPtrs[static_cast<size_t> (i)]->load();

    // Compute band gains and check for changes
    bool changed = false;
    for (int b = 0; b < NUM_BANDS; ++b)
    {
        float gain = calculateBandGainDb (BANDS[static_cast<size_t> (b)], dims);
        if (std::abs (gain - cachedGains[static_cast<size_t> (b)]) > 0.01f)
        {
            cachedGains[static_cast<size_t> (b)] = gain;
            changed = true;
        }
    }

    if (changed)
        repaint();
}

void TeachingPanel::paint (juce::Graphics& g)
{
    if (! expanded)
        return;

    auto area = getLocalBounds();
    area.removeFromTop (36); // skip button area

    auto font = juce::Font (juce::FontOptions (juce::Font::getDefaultMonospacedFontName(), 13.0f, juce::Font::plain));
    g.setFont (font);

    // Column positions (proportional to width)
    auto x = (float) area.getX();
    auto w = (float) area.getWidth();
    float colBand = x + 4.0f;
    float colType = x + w * 0.08f;
    float colFreq = x + w * 0.30f;
    float colGain = x + w * 0.56f;
    float colQ    = x + w * 0.78f;

    float rowY = (float) area.getY();
    float rowH = 22.0f;

    // Header row
    g.setColour (juce::Colour (TonewordLookAndFeel::colTextDim));
    g.drawText ("BAND", juce::Rectangle<float> (colBand, rowY, colType - colBand, rowH), juce::Justification::centredLeft);
    g.drawText ("TYPE", juce::Rectangle<float> (colType, rowY, colFreq - colType, rowH), juce::Justification::centredLeft);
    g.drawText ("FREQ", juce::Rectangle<float> (colFreq, rowY, colGain - colFreq, rowH), juce::Justification::centredLeft);
    g.drawText ("GAIN", juce::Rectangle<float> (colGain, rowY, colQ - colGain, rowH), juce::Justification::centredLeft);
    g.drawText ("Q",    juce::Rectangle<float> (colQ,    rowY, w - (colQ - x), rowH), juce::Justification::centredLeft);
    rowY += rowH;

    // Data rows
    for (int b = 0; b < NUM_BANDS; ++b)
    {
        const auto& band = BANDS[static_cast<size_t> (b)];
        auto rowRect = juce::Rectangle<float> (x, rowY, w, rowH);

        // Alternate row backgrounds
        if (b % 2 == 0)
            g.setColour (juce::Colour (TonewordLookAndFeel::colSurface));
        else
            g.setColour (juce::Colour (TonewordLookAndFeel::colSurface2));
        g.fillRect (rowRect);

        g.setColour (juce::Colour (TonewordLookAndFeel::colText));

        // Band number (1-indexed)
        g.drawText (juce::String (b + 1), juce::Rectangle<float> (colBand, rowY, colType - colBand, rowH), juce::Justification::centredLeft);

        // Filter type
        juce::String typeStr;
        switch (band.type)
        {
            case FilterType::LowShelf:  typeStr = "Low Shelf";  break;
            case FilterType::Peaking:    typeStr = "Peak";       break;
            case FilterType::HighShelf:  typeStr = "High Shelf"; break;
        }
        g.drawText (typeStr, juce::Rectangle<float> (colType, rowY, colFreq - colType, rowH), juce::Justification::centredLeft);

        // Frequency with units
        juce::String freqStr;
        if (band.freq >= 1000.0f)
            freqStr = juce::String (band.freq / 1000.0f, 1) + " kHz";
        else
            freqStr = juce::String ((int) band.freq) + " Hz";
        g.drawText (freqStr, juce::Rectangle<float> (colFreq, rowY, colGain - colFreq, rowH), juce::Justification::centredLeft);

        // Gain with sign
        float gain = cachedGains[static_cast<size_t> (b)];
        juce::String gainStr;
        if (gain > 0.05f)
            gainStr = "+" + juce::String (gain, 1) + " dB";
        else if (gain < -0.05f)
            gainStr = juce::String (gain, 1) + " dB";
        else
            gainStr = "0.0 dB";
        g.drawText (gainStr, juce::Rectangle<float> (colGain, rowY, colQ - colGain, rowH), juce::Justification::centredLeft);

        // Q value
        g.drawText (juce::String (band.baseQ, 2), juce::Rectangle<float> (colQ, rowY, w - (colQ - x), rowH), juce::Justification::centredLeft);

        rowY += rowH;
    }
}
