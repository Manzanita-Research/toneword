#pragma once

#include "../DSP/BandConfig.h"
#include "../Parameters.h"
#include "GUIHelpers.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * Composite EQ frequency response curve panel.
 *
 * Computes the combined magnitude response of all 11 bands entirely
 * on the GUI thread by reading APVTS parameter values (atomic, safe)
 * and using the static BANDS config. No audio-thread state is read.
 *
 * Refreshes at 30Hz but only repaints when parameter values change.
 * Uses float coordinates throughout for HiDPI correctness.
 */
class EQCurvePanel : public juce::Component,
                     private juce::Timer
{
public:
    EQCurvePanel (juce::AudioProcessorValueTreeState& apvts, double sampleRate = 44100.0);
    ~EQCurvePanel() override;

    /** Update the sample rate (call from editor when prepareToPlay fires). */
    void setSampleRate (double newSampleRate);

    void paint (juce::Graphics& g) override;

private:
    void timerCallback() override;

    juce::AudioProcessorValueTreeState& apvtsRef;
    double currentSampleRate;

    // Cached dimension values for dirty-check
    std::array<float, NUM_DIMENSIONS> lastDimValues {};
    bool dirty = true;

    // Atomic parameter value pointers (safe to read from GUI thread)
    std::array<std::atomic<float>*, NUM_DIMENSIONS> paramPtrs {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQCurvePanel)
};
