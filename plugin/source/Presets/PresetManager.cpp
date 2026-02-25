#include "PresetManager.h"

PresetManager::PresetManager (juce::AudioProcessorValueTreeState& apvtsRef)
    : apvts (apvtsRef)
{
}

int PresetManager::getNumPresets() const
{
    return NUM_FACTORY_PRESETS;
}

juce::String PresetManager::getPresetName (int index) const
{
    if (index < 0 || index >= NUM_FACTORY_PRESETS)
        return {};

    return FACTORY_PRESETS[static_cast<size_t> (index)].name;
}

int PresetManager::getCurrentIndex() const
{
    return currentIndex;
}

void PresetManager::setCurrentIndex (int index)
{
    if (index >= 0 && index < NUM_FACTORY_PRESETS)
        currentIndex = index;
}

void PresetManager::loadPreset (int index, bool smooth)
{
    juce::ignoreUnused (smooth);

    if (index < 0 || index >= NUM_FACTORY_PRESETS)
        return;

    const auto& preset = FACTORY_PRESETS[static_cast<size_t> (index)];

    for (int i = 0; i < NUM_DIMENSIONS; ++i)
    {
        auto* param = apvts.getParameter (ParamIDs::allIDs[i]);
        if (param == nullptr)
            continue;

        float presetValue = preset.values[static_cast<size_t> (i)];
        float normalized = param->convertTo0to1 (presetValue);

        param->beginChangeGesture();
        param->setValueNotifyingHost (normalized);
        param->endChangeGesture();
    }

    currentIndex = index;
}

bool PresetManager::isDirty() const
{
    if (currentIndex < 0 || currentIndex >= NUM_FACTORY_PRESETS)
        return false;

    const auto& preset = FACTORY_PRESETS[static_cast<size_t> (currentIndex)];

    for (int i = 0; i < NUM_DIMENSIONS; ++i)
    {
        auto* rawValue = apvts.getRawParameterValue (ParamIDs::allIDs[i]);
        if (rawValue == nullptr)
            continue;

        float current = rawValue->load();
        float expected = preset.values[static_cast<size_t> (i)];

        if (std::abs (current - expected) > 0.5f)
            return true;
    }

    return false;
}

juce::String PresetManager::getCurrentDisplayName() const
{
    if (currentIndex < 0 || currentIndex >= NUM_FACTORY_PRESETS)
        return {};

    juce::String name = FACTORY_PRESETS[static_cast<size_t> (currentIndex)].name;

    if (isDirty())
        name += " *";

    return name;
}

void PresetManager::initializePresetFiles()
{
    auto presetsDir = getPresetsDirectory();

    if (! presetsDir.exists())
        presetsDir.createDirectory();

    for (int i = 0; i < NUM_FACTORY_PRESETS; ++i)
    {
        const auto& preset = FACTORY_PRESETS[static_cast<size_t> (i)];
        auto file = presetsDir.getChildFile (juce::String (preset.name) + ".xml");

        if (file.exists())
            continue;

        juce::XmlElement root ("TonewordPreset");
        root.setAttribute ("name", preset.name);
        root.setAttribute ("version", 1);

        auto* dims = root.createNewChildElement ("Dimensions");
        dims->setAttribute ("warmth", static_cast<double> (preset.values[0]));
        dims->setAttribute ("bite",   static_cast<double> (preset.values[1]));
        dims->setAttribute ("air",    static_cast<double> (preset.values[2]));
        dims->setAttribute ("body",   static_cast<double> (preset.values[3]));
        dims->setAttribute ("glass",  static_cast<double> (preset.values[4]));
        dims->setAttribute ("velvet", static_cast<double> (preset.values[5]));

        root.writeTo (file);
    }
}

juce::File PresetManager::getPresetsDirectory()
{
    return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
        .getChildFile ("Manzanita Research")
        .getChildFile ("Toneword")
        .getChildFile ("Presets");
}
