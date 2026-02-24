#include "SemanticEQ.h"

SemanticEQ::SemanticEQ()
{
    dimensionValues.fill (0.0f);
    currentGainsDb.fill (0.0f);
}

void SemanticEQ::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;

    for (auto& filter : filters)
        filter.prepare (spec);

    updateCoefficients();
}

void SemanticEQ::process (juce::dsp::AudioBlock<float>& block)
{
    if (bypassed)
        return;

    updateCoefficients();

    for (auto& filter : filters)
    {
        juce::dsp::ProcessContextReplacing<float> context (block);
        filter.process (context);
    }
}

void SemanticEQ::reset()
{
    for (auto& filter : filters)
        filter.reset();
}

void SemanticEQ::setDimension (Dimension dim, float value)
{
    dimensionValues[static_cast<size_t> (static_cast<int> (dim))] = value;
}

float SemanticEQ::getBandGainDb (int bandIndex) const
{
    if (bandIndex >= 0 && bandIndex < NUM_BANDS)
        return currentGainsDb[static_cast<size_t> (bandIndex)];
    return 0.0f;
}

void SemanticEQ::setBypassed (bool shouldBypass)
{
    bypassed = shouldBypass;
}

bool SemanticEQ::isBypassed() const
{
    return bypassed;
}

void SemanticEQ::updateCoefficients()
{
    for (int i = 0; i < NUM_BANDS; ++i)
    {
        const auto& band = BANDS[static_cast<size_t> (i)];

        // Calculate gain in dB using the shared formula
        float gainDb = calculateBandGainDb (band, dimensionValues);
        currentGainsDb[static_cast<size_t> (i)] = gainDb;

        // CRITICAL: convert dB to linear for JUCE's IIR coefficient factories
        float gainLinear = juce::Decibels::decibelsToGain (gainDb);

        // Create appropriate filter coefficients
        juce::dsp::IIR::Coefficients<float>::Ptr newCoeffs;

        switch (band.type)
        {
            case FilterType::LowShelf:
                newCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf (
                    sampleRate, band.freq, band.baseQ, gainLinear);
                break;

            case FilterType::Peaking:
                newCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
                    sampleRate, band.freq, band.baseQ, gainLinear);
                break;

            case FilterType::HighShelf:
                newCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf (
                    sampleRate, band.freq, band.baseQ, gainLinear);
                break;
        }

        // Assign coefficients to the filter (safe for audio thread)
        *filters[static_cast<size_t> (i)].state = *newCoeffs;
    }
}
