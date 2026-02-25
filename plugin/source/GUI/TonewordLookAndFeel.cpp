#include "TonewordLookAndFeel.h"

TonewordLookAndFeel::TonewordLookAndFeel()
{
    setColour (juce::ResizableWindow::backgroundColourId, juce::Colour (colBg));
}

void TonewordLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                             float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/,
                                             juce::Slider::SliderStyle /*style*/, juce::Slider& slider)
{
    // Use float coordinates throughout for HiDPI correctness
    auto bounds = juce::Rectangle<float> (static_cast<float> (x),
                                           static_cast<float> (y),
                                           static_cast<float> (width),
                                           static_cast<float> (height));
    auto trackY = bounds.getCentreY();

    // Track: 3px rounded rect in Surface2
    g.setColour (juce::Colour (colSurface2));
    g.fillRoundedRectangle (bounds.getX(), trackY - 1.5f, bounds.getWidth(), 3.0f, 1.5f);

    // Center tick: vertical line at midpoint in Border color (bipolar zero indicator)
    float centerX = bounds.getCentreX();
    g.setColour (juce::Colour (colBorder));
    g.drawLine (centerX, trackY - 5.0f, centerX, trackY + 5.0f, 1.0f);

    // Active fill: colored region from center to thumb position
    auto thumbColor = slider.findColour (juce::Slider::thumbColourId);
    g.setColour (thumbColor.withAlpha (0.35f));

    float fillLeft  = std::min (centerX, sliderPos);
    float fillRight = std::max (centerX, sliderPos);
    g.fillRoundedRectangle (fillLeft, trackY - 1.5f, fillRight - fillLeft, 3.0f, 1.5f);

    // Thumb: 18px filled circle using the slider's thumbColourId
    g.setColour (thumbColor);
    g.fillEllipse (sliderPos - 9.0f, trackY - 9.0f, 18.0f, 18.0f);
}
