#pragma once

#include "BandConfig.h"
#include <juce_dsp/juce_dsp.h>

/**
 * SemanticEQ — 11-band parametric EQ driven by six semantic dimensions.
 *
 * Each dimension (Warmth, Bite, Air, Body, Glass, Velvet) contributes
 * weighted gains to 11 filter bands. Gains are tanh soft-clipped to
 * prevent extreme values, then converted from dB to linear for JUCE's
 * IIR filter coefficients.
 *
 * Ported from web/src/audio/AudioEngine.ts
 */
class SemanticEQ
{
public:
    SemanticEQ();

    /** Prepare the filter chain for processing. */
    void prepare (const juce::dsp::ProcessSpec& spec);

    /** Process an audio block through all 11 filter bands. */
    void process (juce::dsp::AudioBlock<float>& block);

    /** Reset all filter states. */
    void reset();

    /** Set a dimension value (-100 to +100). */
    void setDimension (Dimension dim, float value);

    /** Get the current gain in dB for a band (for UI/testing). */
    float getBandGainDb (int bandIndex) const;

    /** Set bypass mode. When bypassed, process() is a no-op. */
    void setBypassed (bool shouldBypass);

    /** Check bypass state. */
    bool isBypassed() const;

private:
    /** Recalculate filter coefficients from current dimension values. */
    void updateCoefficients();

    // Current dimension values (-100 to +100)
    std::array<float, NUM_DIMENSIONS> dimensionValues {};

    // 11 stereo filter bands
    using FilterBand = juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>>;
    std::array<FilterBand, NUM_BANDS> filters;

    // Cached gain values (dB) for UI readback
    std::array<float, NUM_BANDS> currentGainsDb {};

    double sampleRate = 44100.0;
    bool bypassed = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SemanticEQ)
};
