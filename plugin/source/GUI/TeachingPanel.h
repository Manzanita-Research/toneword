#pragma once

#include "../DSP/BandConfig.h"
#include "../Parameters.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * Collapsible teaching panel showing per-band EQ details.
 *
 * Displays freq, type, gain, and Q for all 11 bands in a monospace
 * table. Gains are computed on the GUI thread from APVTS values
 * using calculateBandGainDb(). Refreshes at 10Hz when expanded.
 */
class TeachingPanel : public juce::Component,
                      private juce::Timer
{
public:
    explicit TeachingPanel (juce::AudioProcessorValueTreeState& apvts);
    ~TeachingPanel() override;

    /** Height needed: 36px collapsed, or 36 + header + 11 rows when expanded. */
    int getPreferredHeight() const;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;

    juce::AudioProcessorValueTreeState& apvtsRef;

    juce::TextButton toggleButton;
    bool expanded = false;

    // Cached band gains for dirty-check repaint
    std::array<float, NUM_BANDS> cachedGains {};

    // Atomic parameter value pointers (safe to read from GUI thread)
    std::array<std::atomic<float>*, NUM_DIMENSIONS> paramPtrs {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TeachingPanel)
};
