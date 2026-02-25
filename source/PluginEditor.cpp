#include "PluginEditor.h"

TonewordAudioProcessorEditor::TonewordAudioProcessorEditor (TonewordAudioProcessor& p)
    : AudioProcessorEditor (p),
      processorRef (p)
{
    // Populate preset ComboBox
    int numPresets = processorRef.getNumPrograms();
    for (int i = 0; i < numPresets; ++i)
        presetBox.addItem (processorRef.getProgramName (i), i + 1); // ComboBox IDs are 1-based

    presetBox.setSelectedId (processorRef.getCurrentProgram() + 1, juce::dontSendNotification);

    presetBox.onChange = [this]()
    {
        int selected = presetBox.getSelectedId();
        if (selected > 0)
            processorRef.setCurrentProgram (selected - 1);
    };

    addAndMakeVisible (presetBox);

    // Prev/next buttons
    prevButton.onClick = [this]()
    {
        int current = processorRef.getCurrentProgram();
        if (current > 0)
            processorRef.setCurrentProgram (current - 1);
    };

    nextButton.onClick = [this]()
    {
        int current = processorRef.getCurrentProgram();
        if (current < processorRef.getNumPrograms() - 1)
            processorRef.setCurrentProgram (current + 1);
    };

    addAndMakeVisible (prevButton);
    addAndMakeVisible (nextButton);

    // Dirty indicator label
    dirtyLabel.setFont (juce::FontOptions (18.0f));
    dirtyLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (dirtyLabel);

    // Embed the generic editor for parameter sliders
    genericEditor = std::make_unique<juce::GenericAudioProcessorEditor> (processorRef);
    addAndMakeVisible (genericEditor.get());

    // Size: slightly taller to fit preset bar above generic editor
    setSize (400, 40 + genericEditor->getHeight());

    startTimerHz (5); // 200ms interval
}

TonewordAudioProcessorEditor::~TonewordAudioProcessorEditor()
{
    stopTimer();
}

void TonewordAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
}

void TonewordAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto topBar = area.removeFromTop (40);

    // Layout: [< 30px] [ComboBox flex] [> 30px] [dirty 20px]
    prevButton.setBounds (topBar.removeFromLeft (30).reduced (2));
    nextButton.setBounds (topBar.removeFromRight (30).reduced (2));
    dirtyLabel.setBounds (topBar.removeFromRight (20));
    presetBox.setBounds (topBar.reduced (2));

    // Generic editor fills the rest
    if (genericEditor != nullptr)
        genericEditor->setBounds (area);
}

void TonewordAudioProcessorEditor::timerCallback()
{
    int current = processorRef.getCurrentProgram();
    presetBox.setSelectedId (current + 1, juce::dontSendNotification);

    bool dirty = processorRef.getPresetManager().isDirty();
    dirtyLabel.setText (dirty ? "*" : "", juce::dontSendNotification);
}
