#include "EQCurvePanel.h"

// Manzanita palette constants
namespace EQPalette
{
    static constexpr juce::uint32 surface   = 0xFF242422;
    static constexpr juce::uint32 border    = 0xFF3A3835;
    static constexpr juce::uint32 textDim   = 0xFF78756F;
    static constexpr juce::uint32 curveLine = 0xFFC8B090; // warm highlight
}

EQCurvePanel::EQCurvePanel (juce::AudioProcessorValueTreeState& apvts, double sampleRate)
    : apvtsRef (apvts),
      currentSampleRate (sampleRate)
{
    // Cache atomic parameter pointers for lock-free reading
    for (int d = 0; d < NUM_DIMENSIONS; ++d)
        paramPtrs[static_cast<size_t> (d)] = apvtsRef.getRawParameterValue (ParamIDs::allIDs[d]);

    startTimerHz (30);
}

EQCurvePanel::~EQCurvePanel()
{
    stopTimer();
}

void EQCurvePanel::setSampleRate (double newSampleRate)
{
    currentSampleRate = newSampleRate;
    dirty = true;
    repaint();
}

void EQCurvePanel::timerCallback()
{
    // Read current dimension values and check if any changed
    bool changed = false;
    for (int d = 0; d < NUM_DIMENSIONS; ++d)
    {
        float val = paramPtrs[static_cast<size_t> (d)]->load (std::memory_order_relaxed);
        if (std::abs (val - lastDimValues[static_cast<size_t> (d)]) > 0.001f)
        {
            lastDimValues[static_cast<size_t> (d)] = val;
            changed = true;
        }
    }

    if (changed || dirty)
    {
        dirty = false;
        repaint();
    }
}

void EQCurvePanel::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float w = bounds.getWidth();
    float h = bounds.getHeight();

    // Dark background
    g.setColour (juce::Colour (EQPalette::surface));
    g.fillRect (bounds);

    // Reserve space for labels
    constexpr float leftMargin = 30.0f;
    constexpr float bottomMargin = 16.0f;
    float plotW = w - leftMargin;
    float plotH = h - bottomMargin;
    float plotX = leftMargin;

    // dB axis labels: -12, -6, 0, +6, +12
    g.setFont (juce::FontOptions (10.0f));
    constexpr float dbRange = 24.0f; // -12 to +12
    struct DbLabel { float db; const char* text; };
    constexpr DbLabel dbLabels[] = {
        { -12.0f, "-12" },
        {  -6.0f,  "-6" },
        {   0.0f,   "0" },
        {   6.0f,  "+6" },
        {  12.0f, "+12" }
    };

    float midY = plotH * 0.5f;

    for (const auto& label : dbLabels)
    {
        float y = midY - (label.db / dbRange) * plotH;
        g.setColour (juce::Colour (EQPalette::textDim));
        g.drawText (label.text, juce::Rectangle<float> (0.0f, y - 6.0f, leftMargin - 4.0f, 12.0f),
                    juce::Justification::centredRight, false);

        // Grid line
        if (label.db == 0.0f)
        {
            g.setColour (juce::Colour (EQPalette::border).withAlpha (0.8f));
            g.drawLine (plotX, y, w, y, 1.0f);
        }
        else
        {
            g.setColour (juce::Colour (EQPalette::border).withAlpha (0.3f));
            g.drawLine (plotX, y, w, y, 0.5f);
        }
    }

    // Frequency axis labels: 100, 1k, 10k
    struct FreqLabel { float freq; const char* text; };
    constexpr FreqLabel freqLabels[] = {
        { 100.0f, "100" },
        { 1000.0f, "1k" },
        { 10000.0f, "10k" }
    };

    for (const auto& label : freqLabels)
    {
        float x = plotX + GUIHelpers::freqToX (label.freq, plotW);
        // Vertical grid line
        g.setColour (juce::Colour (EQPalette::border).withAlpha (0.3f));
        g.drawLine (x, 0.0f, x, plotH, 0.5f);
        // Label text
        g.setColour (juce::Colour (EQPalette::textDim));
        g.drawText (label.text, juce::Rectangle<float> (x - 15.0f, plotH + 1.0f, 30.0f, bottomMargin - 1.0f),
                    juce::Justification::centred, false);
    }

    // Compute composite frequency response on the GUI thread
    constexpr int numPoints = 256;
    std::array<float, numPoints> responseDb {};
    responseDb.fill (0.0f);

    // Read current dimension values
    std::array<float, NUM_DIMENSIONS> dims {};
    for (int d = 0; d < NUM_DIMENSIONS; ++d)
        dims[static_cast<size_t> (d)] = paramPtrs[static_cast<size_t> (d)]->load (std::memory_order_relaxed);

    // Generate log-spaced frequencies
    std::array<double, numPoints> freqs {};
    for (int i = 0; i < numPoints; ++i)
        freqs[static_cast<size_t> (i)] = 20.0 * std::pow (1000.0, (double) i / (double) (numPoints - 1));

    // Sum magnitude response from all 11 bands
    for (int band = 0; band < NUM_BANDS; ++band)
    {
        const auto& bc = BANDS[static_cast<size_t> (band)];
        float gainDb = calculateBandGainDb (bc, dims);
        float gainLinear = juce::Decibels::decibelsToGain (gainDb);

        // Create temporary IIR coefficients for this band
        juce::dsp::IIR::Coefficients<float>::Ptr coeffs;

        switch (bc.type)
        {
            case FilterType::LowShelf:
                coeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf (
                    currentSampleRate, bc.freq, bc.baseQ, gainLinear);
                break;
            case FilterType::Peaking:
                coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
                    currentSampleRate, bc.freq, bc.baseQ, gainLinear);
                break;
            case FilterType::HighShelf:
                coeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf (
                    currentSampleRate, bc.freq, bc.baseQ, gainLinear);
                break;
        }

        // Query magnitude at each frequency
        std::array<double, numPoints> mags {};
        coeffs->getMagnitudeForFrequencyArray (freqs.data(), mags.data(), numPoints, currentSampleRate);

        // Accumulate in dB (multiply magnitudes in linear = add in dB)
        for (int i = 0; i < numPoints; ++i)
            responseDb[static_cast<size_t> (i)] += (float) juce::Decibels::gainToDecibels (mags[static_cast<size_t> (i)]);
    }

    // Draw the composite response curve as a filled path
    juce::Path curvePath;
    bool pathStarted = false;

    for (int i = 0; i < numPoints; ++i)
    {
        float freq = (float) freqs[static_cast<size_t> (i)];
        float x = plotX + GUIHelpers::freqToX (freq, plotW);
        float y = midY - (responseDb[static_cast<size_t> (i)] / dbRange) * plotH;

        // Clamp y to plot area
        y = juce::jlimit (0.0f, plotH, y);

        if (! pathStarted)
        {
            curvePath.startNewSubPath (x, y);
            pathStarted = true;
        }
        else
        {
            curvePath.lineTo (x, y);
        }
    }

    if (pathStarted)
    {
        // Stroke the curve line
        auto lineColor = juce::Colour (EQPalette::curveLine);
        g.setColour (lineColor);
        g.strokePath (curvePath, juce::PathStrokeType (2.0f));

        // Create filled version (close along zero-dB line)
        juce::Path fillPath (curvePath);
        fillPath.lineTo (plotX + GUIHelpers::freqToX (20000.0f, plotW), midY);
        fillPath.lineTo (plotX + GUIHelpers::freqToX (20.0f, plotW), midY);
        fillPath.closeSubPath();

        // Gradient fill from curve color at 40% to transparent
        g.setGradientFill (juce::ColourGradient (
            lineColor.withAlpha (0.4f), 0.0f, midY - plotH * 0.3f,
            lineColor.withAlpha (0.0f), 0.0f, midY,
            false));
        g.fillPath (fillPath);
    }
}
