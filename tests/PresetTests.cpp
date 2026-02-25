#include <PluginProcessor.h>
#include <Parameters.h>
#include <Presets/FactoryPresets.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE ("Factory presets count is 9", "[presets]")
{
    CHECK (NUM_FACTORY_PRESETS == 9);
}

TEST_CASE ("Init preset has all zeros", "[presets]")
{
    for (int i = 0; i < NUM_DIMENSIONS; ++i)
        CHECK (FACTORY_PRESETS[0].values[static_cast<size_t> (i)] == 0.0f);
}

TEST_CASE ("All factory preset names are non-empty", "[presets]")
{
    for (int i = 0; i < NUM_FACTORY_PRESETS; ++i)
    {
        INFO ("Preset index: " << i);
        CHECK (juce::String (FACTORY_PRESETS[static_cast<size_t> (i)].name).isNotEmpty());
    }
}

TEST_CASE ("Factory preset names match spec", "[presets]")
{
    CHECK_THAT (std::string (FACTORY_PRESETS[0].name), Catch::Matchers::Equals ("Init"));
    CHECK_THAT (std::string (FACTORY_PRESETS[1].name), Catch::Matchers::Equals ("Nashville Twang"));
    CHECK_THAT (std::string (FACTORY_PRESETS[2].name), Catch::Matchers::Equals ("Midnight Jazz"));
    CHECK_THAT (std::string (FACTORY_PRESETS[3].name), Catch::Matchers::Equals ("Punk Scoop"));
    CHECK_THAT (std::string (FACTORY_PRESETS[4].name), Catch::Matchers::Equals ("Bedroom Producer"));
    CHECK_THAT (std::string (FACTORY_PRESETS[5].name), Catch::Matchers::Equals ("Blues Bark"));
    CHECK_THAT (std::string (FACTORY_PRESETS[6].name), Catch::Matchers::Equals ("Shoegaze Wash"));
    CHECK_THAT (std::string (FACTORY_PRESETS[7].name), Catch::Matchers::Equals ("Country Sparkle"));
    CHECK_THAT (std::string (FACTORY_PRESETS[8].name), Catch::Matchers::Equals ("Neo Soul Cream"));
}

TEST_CASE ("PresetManager reports correct count", "[presets]")
{
    TonewordAudioProcessor testPlugin;
    CHECK (testPlugin.getNumPrograms() == 9);
}

TEST_CASE ("PresetManager returns correct names", "[presets]")
{
    TonewordAudioProcessor testPlugin;
    CHECK_THAT (testPlugin.getProgramName (0).toStdString(), Catch::Matchers::Equals ("Init"));
    CHECK_THAT (testPlugin.getProgramName (1).toStdString(), Catch::Matchers::Equals ("Nashville Twang"));
    CHECK_THAT (testPlugin.getProgramName (2).toStdString(), Catch::Matchers::Equals ("Midnight Jazz"));
    CHECK_THAT (testPlugin.getProgramName (3).toStdString(), Catch::Matchers::Equals ("Punk Scoop"));
    CHECK_THAT (testPlugin.getProgramName (4).toStdString(), Catch::Matchers::Equals ("Bedroom Producer"));
    CHECK_THAT (testPlugin.getProgramName (5).toStdString(), Catch::Matchers::Equals ("Blues Bark"));
    CHECK_THAT (testPlugin.getProgramName (6).toStdString(), Catch::Matchers::Equals ("Shoegaze Wash"));
    CHECK_THAT (testPlugin.getProgramName (7).toStdString(), Catch::Matchers::Equals ("Country Sparkle"));
    CHECK_THAT (testPlugin.getProgramName (8).toStdString(), Catch::Matchers::Equals ("Neo Soul Cream"));
}

TEST_CASE ("Loading preset changes parameter values", "[presets]")
{
    TonewordAudioProcessor testPlugin;

    // Load Nashville Twang (index 1) — warmth should be -20
    testPlugin.setCurrentProgram (1);

    auto* rawWarmth = testPlugin.getAPVTS().getRawParameterValue (ParamIDs::Warmth);
    REQUIRE (rawWarmth != nullptr);
    CHECK_THAT (static_cast<double> (rawWarmth->load()),
        Catch::Matchers::WithinAbs (-20.0, 0.5));
}

TEST_CASE ("Init preset is not dirty after load", "[presets]")
{
    TonewordAudioProcessor testPlugin;

    // Load Init preset (index 0)
    testPlugin.setCurrentProgram (0);

    CHECK (testPlugin.getPresetManager().isDirty() == false);
}
