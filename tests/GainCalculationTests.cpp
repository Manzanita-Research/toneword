#include <DSP/BandConfig.h>
#include <DSP/SemanticEQ.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <juce_audio_processors/juce_audio_processors.h>

using Catch::Matchers::WithinAbs;

// Helper: create a dimension array with all zeros
static std::array<float, NUM_DIMENSIONS> flatState()
{
    return { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
}

// Helper: create a state with one dimension set
static std::array<float, NUM_DIMENSIONS> singleDim (Dimension dim, float value)
{
    auto state = flatState();
    state[static_cast<int> (dim)] = value;
    return state;
}

TEST_CASE ("Flat state produces zero gain on all bands", "[gain]")
{
    auto state = flatState();

    for (int i = 0; i < NUM_BANDS; ++i)
    {
        float gain = calculateBandGainDb (BANDS[static_cast<size_t> (i)], state);
        CHECK_THAT (gain, WithinAbs (0.0f, 0.001f));
    }
}

TEST_CASE ("Single dimension warmth at +100 produces expected gains", "[gain]")
{
    auto state = singleDim (Dimension::Warmth, 100.0f);

    // Band 2 (300Hz): weight 0.9 -> g = (100/100)*0.9*12 = 10.8 -> tanh(10.8/12)*12
    float expected300 = std::tanh (10.8f / MAX_GAIN_DB) * MAX_GAIN_DB;
    CHECK_THAT (calculateBandGainDb (BANDS[2], state), WithinAbs (expected300, 0.01f));

    // Band 0 (60Hz lowshelf): weight 0.2 -> g = (100/100)*0.2*12 = 2.4 -> tanh(2.4/12)*12
    float expected60 = std::tanh (2.4f / MAX_GAIN_DB) * MAX_GAIN_DB;
    CHECK_THAT (calculateBandGainDb (BANDS[0], state), WithinAbs (expected60, 0.01f));

    // Band 6 (2500Hz): weight -0.5 -> g = (100/100)*(-0.5)*12 = -6.0 -> tanh(-6.0/12)*12
    float expected2500 = std::tanh (-6.0f / MAX_GAIN_DB) * MAX_GAIN_DB;
    CHECK_THAT (calculateBandGainDb (BANDS[6], state), WithinAbs (expected2500, 0.01f));
}

TEST_CASE ("Cross-coupling: Glass positive affects Velvet-weighted bands", "[gain]")
{
    auto state = singleDim (Dimension::Glass, 100.0f);

    // Band 6 (2500Hz): glass weight 0.6, velvet weight -0.8
    // With only glass=100: g = (100/100)*0.6*12 = 7.2
    // (velvet term is 0 because velvet dim = 0)
    // tanh(7.2/12)*12
    float expected = std::tanh (7.2f / MAX_GAIN_DB) * MAX_GAIN_DB;
    CHECK_THAT (calculateBandGainDb (BANDS[6], state), WithinAbs (expected, 0.01f));

    // Band 4 (800Hz): glass weight 0.4
    // g = (100/100)*0.4*12 = 4.8 -> tanh(4.8/12)*12
    float expected800 = std::tanh (4.8f / MAX_GAIN_DB) * MAX_GAIN_DB;
    CHECK_THAT (calculateBandGainDb (BANDS[4], state), WithinAbs (expected800, 0.01f));
}

TEST_CASE ("tanh clipping keeps gain within +-MAX_GAIN", "[gain]")
{
    // All dimensions at +100 — max possible stress
    std::array<float, NUM_DIMENSIONS> maxState;
    maxState.fill (100.0f);

    for (int i = 0; i < NUM_BANDS; ++i)
    {
        float gain = calculateBandGainDb (BANDS[static_cast<size_t> (i)], maxState);
        CHECK (std::abs (gain) <= MAX_GAIN_DB);
    }

    // All dimensions at -100
    std::array<float, NUM_DIMENSIONS> minState;
    minState.fill (-100.0f);

    for (int i = 0; i < NUM_BANDS; ++i)
    {
        float gain = calculateBandGainDb (BANDS[static_cast<size_t> (i)], minState);
        CHECK (std::abs (gain) <= MAX_GAIN_DB);
    }
}

TEST_CASE ("dB to linear conversion is correct", "[gain]")
{
    // 6 dB should be approximately 2.0 linear
    CHECK_THAT (juce::Decibels::decibelsToGain (6.0f), WithinAbs (2.0f, 0.02f));

    // -6 dB should be approximately 0.5 linear
    CHECK_THAT (juce::Decibels::decibelsToGain (-6.0f), WithinAbs (0.5f, 0.02f));

    // 0 dB should be exactly 1.0 linear
    CHECK_THAT (juce::Decibels::decibelsToGain (0.0f), WithinAbs (1.0f, 0.001f));
}

TEST_CASE ("Bipolar: negative dimension reverses gain direction", "[gain]")
{
    auto positive = singleDim (Dimension::Warmth, 100.0f);
    auto negative = singleDim (Dimension::Warmth, -100.0f);

    for (int i = 0; i < NUM_BANDS; ++i)
    {
        float gainPos = calculateBandGainDb (BANDS[static_cast<size_t> (i)], positive);
        float gainNeg = calculateBandGainDb (BANDS[static_cast<size_t> (i)], negative);

        // Signs should be opposite (or both zero)
        if (std::abs (gainPos) > 0.001f)
        {
            CHECK (gainPos * gainNeg < 0.0f);
        }

        // Magnitudes should be equal (tanh is an odd function)
        CHECK_THAT (std::abs (gainPos), WithinAbs (std::abs (gainNeg), 0.001f));
    }
}
