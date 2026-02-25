#pragma once

#include "PluginProcessor.h"
#include "GUI/TonewordLookAndFeel.h"
#include "GUI/SpectrumPanel.h"
#include "GUI/EQCurvePanel.h"
#include "GUI/SliderPanel.h"
#include "GUI/TeachingPanel.h"
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

    // LookAndFeel must outlive all child components
    TonewordLookAndFeel lookAndFeel;

    // Preset bar
    juce::ComboBox presetBox;
    juce::TextButton prevButton { "<" };
    juce::TextButton nextButton { ">" };
    juce::Label dirtyLabel;

    // Section labels
    juce::Label spectrumLabel;
    juce::Label toneWordsLabel;

    // Visualization panels
    SpectrumPanel spectrumPanel;
    EQCurvePanel eqCurvePanel;

    // Controls
    SliderPanel sliderPanel;
    TeachingPanel teachingPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TonewordAudioProcessorEditor)
};
