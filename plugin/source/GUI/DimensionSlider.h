#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * A single dimension control: colored label, description text,
 * current value display, and a horizontal slider with APVTS attachment.
 */
class DimensionSlider : public juce::Component
{
public:
    DimensionSlider (juce::AudioProcessorValueTreeState& apvts,
                     const juce::String& paramID,
                     const juce::String& name,
                     const juce::String& description,
                     juce::Colour color);

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    juce::Colour dimColor;
    juce::String dimName;
    juce::String dimDesc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DimensionSlider)
};
