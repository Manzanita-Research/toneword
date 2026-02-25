#include <DSP/BandConfig.h>
#include <DSP/SemanticEQ.h>
#include <catch2/catch_test_macros.hpp>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <random>

TEST_CASE ("Filter chain processes stereo audio without NaN or Inf", "[filter]")
{
    SemanticEQ eq;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    eq.prepare (spec);

    // Set some non-zero state
    eq.setDimension (Dimension::Warmth, 75.0f);
    eq.setDimension (Dimension::Glass, -50.0f);
    eq.setDimension (Dimension::Air, 30.0f);

    // Create a buffer of white noise
    juce::AudioBuffer<float> buffer (2, 512);
    std::mt19937 rng (42); // fixed seed for reproducibility
    std::uniform_real_distribution<float> dist (-1.0f, 1.0f);

    for (int ch = 0; ch < 2; ++ch)
        for (int s = 0; s < 512; ++s)
            buffer.setSample (ch, s, dist (rng));

    // Process
    juce::dsp::AudioBlock<float> block (buffer);
    eq.process (block);

    // Verify no NaN or Inf in output
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* samples = buffer.getReadPointer (ch);
        for (int s = 0; s < 512; ++s)
        {
            CHECK_FALSE (std::isnan (samples[s]));
            CHECK_FALSE (std::isinf (samples[s]));
        }
    }
}

TEST_CASE ("Bypass produces bit-identical output", "[filter]")
{
    SemanticEQ eq;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = 44100.0;
    spec.maximumBlockSize = 512;
    spec.numChannels = 2;
    eq.prepare (spec);

    // Set non-zero state so filters would change audio if not bypassed
    eq.setDimension (Dimension::Bite, 80.0f);
    eq.setDimension (Dimension::Velvet, -60.0f);

    // Enable bypass
    eq.setBypassed (true);

    // Create a known buffer
    juce::AudioBuffer<float> buffer (2, 512);
    std::mt19937 rng (123);
    std::uniform_real_distribution<float> dist (-1.0f, 1.0f);

    for (int ch = 0; ch < 2; ++ch)
        for (int s = 0; s < 512; ++s)
            buffer.setSample (ch, s, dist (rng));

    // Copy the original buffer
    juce::AudioBuffer<float> original (2, 512);
    for (int ch = 0; ch < 2; ++ch)
        original.copyFrom (ch, 0, buffer, ch, 0, 512);

    // Process (bypassed)
    juce::dsp::AudioBlock<float> block (buffer);
    eq.process (block);

    // Output must be bit-identical to input
    for (int ch = 0; ch < 2; ++ch)
    {
        auto* out = buffer.getReadPointer (ch);
        auto* orig = original.getReadPointer (ch);
        for (int s = 0; s < 512; ++s)
        {
            CHECK (out[s] == orig[s]);
        }
    }
}
