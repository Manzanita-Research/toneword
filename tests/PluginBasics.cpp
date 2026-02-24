#include <PluginProcessor.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

TEST_CASE ("Plugin instance name", "[instance]")
{
    TonewordAudioProcessor testPlugin;

    CHECK_THAT (testPlugin.getName().toStdString(),
        Catch::Matchers::Equals ("Toneword"));
}

TEST_CASE ("Plugin accepts stereo", "[instance]")
{
    TonewordAudioProcessor testPlugin;
    CHECK (testPlugin.isBusesLayoutSupported (
        { { juce::AudioChannelSet::stereo() }, { juce::AudioChannelSet::stereo() } }));
}
