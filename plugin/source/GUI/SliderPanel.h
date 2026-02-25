#pragma once

#include "DimensionSlider.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <array>

/**
 * 2x3 grid of six DimensionSliders — the primary control surface.
 * Grid order: Warmth/Bite (row 1), Air/Body (row 2), Glass/Velvet (row 3).
 */
class SliderPanel : public juce::Component
{
public:
    explicit SliderPanel (juce::AudioProcessorValueTreeState& apvts);

    void resized() override;

private:
    static constexpr int numSliders = 6;
    std::array<std::unique_ptr<DimensionSlider>, numSliders> sliders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderPanel)
};
