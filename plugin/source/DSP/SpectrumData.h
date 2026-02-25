#pragma once

#include <array>
#include <atomic>
#include <juce_dsp/juce_dsp.h>

/**
 * Lock-free FIFO + FFT for spectrum visualization.
 *
 * Single-producer (audio thread pushes samples via pushSample),
 * single-consumer (GUI thread polls isNextBlockReady, calls processBlock).
 * No locks needed — the atomic bool flag is the only synchronization.
 *
 * FFT order 11 = 2048 points. Output is 512 scope bins in normalized
 * 0-1 range (mapped from -100dB to 0dB). Temporal smoothing applied
 * to reduce frame-to-frame jitter.
 */
class SpectrumData
{
public:
    static constexpr int fftOrder = 11;
    static constexpr int fftSize  = 1 << fftOrder; // 2048
    static constexpr int scopeSize = 512;

    SpectrumData() = default;

    /** Push a single sample from the audio thread. */
    void pushSample (float sample)
    {
        if (fifoIndex == fftSize)
        {
            std::copy (fifo.begin(), fifo.end(), fftData.begin());
            fifoIndex = 0;
            nextBlockReady = true;
        }
        fifo[static_cast<size_t> (fifoIndex++)] = sample;
    }

    /** Check if a new FFT block is ready (call from GUI thread). */
    bool isNextBlockReady() const { return nextBlockReady.load (std::memory_order_relaxed); }

    /**
     * Process the FFT data into scope bins (call from GUI thread only).
     * Applies Hann window, FFT, log-scale dB conversion, and temporal smoothing.
     */
    void processBlock()
    {
        // Apply Hann window
        window.multiplyWithWindowingTable (fftData.data(), fftSize);

        // Forward FFT (frequency-only, magnitudes in first half)
        fft.performFrequencyOnlyForwardTransform (fftData.data());

        // Convert to log-spaced scope data in normalized 0-1 range
        for (int i = 0; i < scopeSize; ++i)
        {
            // Skew index for log-frequency distribution
            auto skewedProportion = 1.0f - std::exp (std::log (1.0f - (float) i / (float) scopeSize) * 0.2f);
            auto fftIndex = static_cast<size_t> (skewedProportion * (float) fftSize * 0.5f);

            auto level = juce::Decibels::gainToDecibels (fftData[fftIndex])
                       - juce::Decibels::gainToDecibels ((float) fftSize);

            // Map -100dB..0dB to 0..1
            float newValue = juce::jmap (level, -100.0f, 0.0f, 0.0f, 1.0f);

            // Temporal smoothing: 80% old + 20% new (reduces jitter)
            scopeData[static_cast<size_t> (i)] = 0.8f * scopeData[static_cast<size_t> (i)] + 0.2f * newValue;
        }

        nextBlockReady = false;
    }

    /** Get the current scope data (512 bins, each 0-1). */
    const std::array<float, scopeSize>& getScopeData() const { return scopeData; }

private:
    juce::dsp::FFT fft { fftOrder };
    juce::dsp::WindowingFunction<float> window { fftSize, juce::dsp::WindowingFunction<float>::hann };

    std::array<float, fftSize> fifo {};
    std::array<float, fftSize * 2> fftData {};
    std::array<float, scopeSize> scopeData {};
    int fifoIndex = 0;
    std::atomic<bool> nextBlockReady { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumData)
};
