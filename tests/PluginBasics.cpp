#include <PluginProcessor.h>
#include <Parameters.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

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

TEST_CASE ("Plugin has bypass parameter", "[parameters]")
{
    TonewordAudioProcessor testPlugin;
    auto* bypassParam = testPlugin.getBypassParameter();
    REQUIRE (bypassParam != nullptr);
    CHECK_THAT (bypassParam->getName (128).toStdString(),
        Catch::Matchers::ContainsSubstring ("Bypass"));
}

TEST_CASE ("Plugin has snap/smooth parameter", "[parameters]")
{
    TonewordAudioProcessor testPlugin;
    auto& apvts = testPlugin.getAPVTS();
    auto* param = apvts.getParameter ("snap_smooth");
    REQUIRE (param != nullptr);
}

TEST_CASE ("Plugin has 8 total parameters", "[parameters]")
{
    TonewordAudioProcessor testPlugin;
    auto& apvts = testPlugin.getAPVTS();

    CHECK (apvts.getParameter (ParamIDs::Warmth) != nullptr);
    CHECK (apvts.getParameter (ParamIDs::Bite)   != nullptr);
    CHECK (apvts.getParameter (ParamIDs::Air)    != nullptr);
    CHECK (apvts.getParameter (ParamIDs::Body)   != nullptr);
    CHECK (apvts.getParameter (ParamIDs::Glass)  != nullptr);
    CHECK (apvts.getParameter (ParamIDs::Velvet) != nullptr);
    CHECK (apvts.getParameter (ParamIDs::Bypass) != nullptr);
    CHECK (apvts.getParameter (ParamIDs::SnapSmooth) != nullptr);
}

TEST_CASE ("State round-trip preserves values", "[state]")
{
    // Save state from processor A with warmth set to 50
    juce::MemoryBlock stateData;
    {
        TonewordAudioProcessor procA;
        auto* warmthParam = procA.getAPVTS().getParameter (ParamIDs::Warmth);
        REQUIRE (warmthParam != nullptr);

        // setValueNotifyingHost takes normalized 0..1 value
        // warmth range is -100 to 100, so 50 = (50 - (-100)) / (100 - (-100)) = 150/200 = 0.75
        warmthParam->setValueNotifyingHost (0.75f);

        procA.getStateInformation (stateData);
    }

    // Restore into processor B and verify
    {
        TonewordAudioProcessor procB;
        procB.setStateInformation (stateData.getData(), static_cast<int> (stateData.getSize()));

        auto* rawWarmth = procB.getAPVTS().getRawParameterValue (ParamIDs::Warmth);
        REQUIRE (rawWarmth != nullptr);
        CHECK_THAT (static_cast<double> (rawWarmth->load()),
            Catch::Matchers::WithinAbs (50.0, 0.5));
    }
}

TEST_CASE ("getNumPrograms returns at least 1", "[programs]")
{
    TonewordAudioProcessor testPlugin;
    CHECK (testPlugin.getNumPrograms() >= 1);
}
