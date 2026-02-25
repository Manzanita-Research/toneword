#pragma once

#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class TonewordAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     private juce::Timer
{
public:
    explicit TonewordAudioProcessorEditor (TonewordAudioProcessor&);
    ~TonewordAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    TonewordAudioProcessor& processorRef;

    juce::ComboBox presetBox;
    juce::TextButton prevButton { "<" };
    juce::TextButton nextButton { ">" };
    juce::Label dirtyLabel;

    std::unique_ptr<juce::GenericAudioProcessorEditor> genericEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TonewordAudioProcessorEditor)
};
