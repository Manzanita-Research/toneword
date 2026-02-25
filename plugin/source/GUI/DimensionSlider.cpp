#include "DimensionSlider.h"
#include "TonewordLookAndFeel.h"

DimensionSlider::DimensionSlider (juce::AudioProcessorValueTreeState& apvts,
                                   const juce::String& paramID,
                                   const juce::String& name,
                                   const juce::String& description,
                                   juce::Colour color)
    : dimColor (color), dimName (name), dimDesc (description)
{
    slider.setSliderStyle (juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
    slider.setRange (-100.0, 100.0, 0.01);
    slider.setDoubleClickReturnValue (true, 0.0);
    slider.setColour (juce::Slider::thumbColourId, dimColor);
    addAndMakeVisible (slider);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        apvts, paramID, slider);
}

void DimensionSlider::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    auto textArea = bounds.removeFromTop (40.0f);

    // Dimension name — left-aligned, colored, bold
    auto nameFont = juce::FontOptions (15.0f);
    g.setFont (juce::Font (nameFont).boldened());
    g.setColour (dimColor);
    g.drawText (dimName, textArea.removeFromTop (20.0f).toNearestInt(),
                juce::Justification::centredLeft, false);

    // Description — below name, dim text, smaller
    auto descFont = juce::FontOptions (11.5f);
    g.setFont (juce::Font (descFont));
    g.setColour (juce::Colour (TonewordLookAndFeel::colTextDim));
    g.drawText (dimDesc, textArea.toNearestInt(),
                juce::Justification::centredLeft, false);

    // Current value display — right-aligned at top
    auto valueArea = getLocalBounds().toFloat().removeFromTop (20.0f);
    auto val = slider.getValue();
    juce::String valueText;
    if (val > 0.0)
        valueText = "+" + juce::String (static_cast<int> (std::round (val)));
    else
        valueText = juce::String (static_cast<int> (std::round (val)));

    auto valueFont = juce::FontOptions (14.0f);
    g.setFont (juce::Font (valueFont));
    g.setColour (juce::Colour (TonewordLookAndFeel::colText));
    g.drawText (valueText, valueArea.toNearestInt(),
                juce::Justification::centredRight, false);
}

void DimensionSlider::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop (40); // space for name/description/value
    slider.setBounds (bounds);
}
