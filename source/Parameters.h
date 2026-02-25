#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

/**
 * Parameter IDs for the six semantic dimensions.
 * These MUST remain stable across all future versions —
 * they are baked into DAW session files.
 */
namespace ParamIDs
{
    inline constexpr const char* Warmth = "warmth";
    inline constexpr const char* Bite   = "bite";
    inline constexpr const char* Air    = "air";
    inline constexpr const char* Body   = "body";
    inline constexpr const char* Glass  = "glass";
    inline constexpr const char* Velvet = "velvet";

    inline constexpr const char* Bypass    = "bypass";
    inline constexpr const char* SnapSmooth = "snap_smooth";

    /** All IDs in dimension-enum order (matches Dimension enum). */
    inline constexpr const char* allIDs[] = { Warmth, Bite, Air, Body, Glass, Velvet };
}

/**
 * Create the APVTS parameter layout for the six semantic dimensions.
 * Each is a float parameter, -100 to +100, default 0.
 */
inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto range = juce::NormalisableRange<float> (-100.0f, 100.0f, 0.01f);

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::Warmth, 1 }, "Warmth", range, 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::Bite, 1 }, "Bite", range, 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::Air, 1 }, "Air", range, 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::Body, 1 }, "Body", range, 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::Glass, 1 }, "Glass", range, 0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::Velvet, 1 }, "Velvet", range, 0.0f));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::Bypass, 1 }, "Bypass", false));

    layout.add (std::make_unique<juce::AudioParameterBool> (
        juce::ParameterID { ParamIDs::SnapSmooth, 1 }, "Snap/Smooth", false,
        juce::AudioParameterBoolAttributes()
            .withStringFromValueFunction ([] (bool value, int) { return value ? juce::String ("Smooth") : juce::String ("Snap"); })));

    return layout;
}
