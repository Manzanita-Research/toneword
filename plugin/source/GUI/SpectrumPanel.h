#pragma once

#include "../DSP/SpectrumData.h"
#include "GUIHelpers.h"
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * Real-time spectrum analyzer panel.
 *
 * Polls SpectrumData at 30Hz and draws a filled frequency-domain
 * visualization on a warm dark background with log-scale axis labels.
 * Uses float coordinates throughout for HiDPI correctness.
 */
class SpectrumPanel : public juce::Component,
                      private juce::Timer
{
public:
    explicit SpectrumPanel (SpectrumData& data);
    ~SpectrumPanel() override;

    void paint (juce::Graphics& g) override;

private:
    void timerCallback() override;

    SpectrumData& spectrumData;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumPanel)
};
