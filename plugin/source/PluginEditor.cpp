#include "PluginEditor.h"

TonewordAudioProcessorEditor::TonewordAudioProcessorEditor (TonewordAudioProcessor& p)
    : AudioProcessorEditor (p),
      processorRef (p),
      spectrumPanel (p.getSpectrumData()),
      eqCurvePanel (p.getAPVTS(), p.getSampleRate()),
      sliderPanel (p.getAPVTS()),
      teachingPanel (p.getAPVTS())
{
    setLookAndFeel (&lookAndFeel);

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

    // Dirty indicator
    dirtyLabel.setFont (juce::FontOptions (18.0f));
    dirtyLabel.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (dirtyLabel);

    // Section labels — uppercase monospace, dim text
    auto sectionFont = juce::Font (juce::FontOptions (juce::Font::getDefaultMonospacedFontName(), 12.0f, juce::Font::plain));

    spectrumLabel.setText ("SPECTRUM", juce::dontSendNotification);
    spectrumLabel.setFont (sectionFont);
    spectrumLabel.setColour (juce::Label::textColourId, juce::Colour (TonewordLookAndFeel::colTextDim));
    addAndMakeVisible (spectrumLabel);

    toneWordsLabel.setText ("TONE WORDS", juce::dontSendNotification);
    toneWordsLabel.setFont (sectionFont);
    toneWordsLabel.setColour (juce::Label::textColourId, juce::Colour (TonewordLookAndFeel::colTextDim));
    addAndMakeVisible (toneWordsLabel);

    // Visualization and control panels
    addAndMakeVisible (spectrumPanel);
    addAndMakeVisible (eqCurvePanel);
    addAndMakeVisible (sliderPanel);
    addAndMakeVisible (teachingPanel);

    // Resizable window with proportional layout
    setResizable (true, true);
    setResizeLimits (500, 600, 1200, 1400);
    setSize (700, 900);

    startTimerHz (5); // 5Hz for preset sync
}

TonewordAudioProcessorEditor::~TonewordAudioProcessorEditor()
{
    stopTimer();
    // LookAndFeel must be cleared before child components are destroyed
    setLookAndFeel (nullptr);
}

void TonewordAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (TonewordLookAndFeel::colBg));
}

void TonewordAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (12);

    constexpr int headerH = 36;
    constexpr int labelH = 16;
    constexpr int gap = 8;

    // Preset bar
    auto presetBar = area.removeFromTop (headerH);
    prevButton.setBounds (presetBar.removeFromLeft (30).reduced (2));
    nextButton.setBounds (presetBar.removeFromRight (30).reduced (2));
    dirtyLabel.setBounds (presetBar.removeFromRight (20));
    presetBox.setBounds (presetBar.reduced (2));
    area.removeFromTop (gap);

    // Calculate proportional heights from remaining space
    auto remainingH = area.getHeight();
    auto specH  = (int) ((float) remainingH * 0.18f);
    auto eqH    = (int) ((float) remainingH * 0.15f);
    auto sliderH = (int) ((float) remainingH * 0.35f);

    // "SPECTRUM" section label
    spectrumLabel.setBounds (area.removeFromTop (labelH));
    spectrumPanel.setBounds (area.removeFromTop (specH));
    area.removeFromTop (gap);

    // EQ curve
    eqCurvePanel.setBounds (area.removeFromTop (eqH));
    area.removeFromTop (gap);

    // "TONE WORDS" section label
    toneWordsLabel.setBounds (area.removeFromTop (labelH));
    sliderPanel.setBounds (area.removeFromTop (sliderH));
    area.removeFromTop (gap);

    // Teaching panel fills remaining space
    teachingPanel.setBounds (area);
}

void TonewordAudioProcessorEditor::timerCallback()
{
    // Sync preset ComboBox selection
    int current = processorRef.getCurrentProgram();
    presetBox.setSelectedId (current + 1, juce::dontSendNotification);

    // Dirty indicator
    bool dirty = processorRef.getPresetManager().isDirty();
    dirtyLabel.setText (dirty ? "*" : "", juce::dontSendNotification);
}
