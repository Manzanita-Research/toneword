#pragma once

#include "FactoryPresets.h"
#include "../Parameters.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

/**
 * Manages factory presets — loading into APVTS, dirty detection,
 * disk-based XML storage, and DAW program interface support.
 */
class PresetManager
{
public:
    explicit PresetManager (juce::AudioProcessorValueTreeState& apvts);

    int getNumPresets() const;
    juce::String getPresetName (int index) const;

    int getCurrentIndex() const;
    void setCurrentIndex (int index);

    /**
     * Load a preset's values into the APVTS parameters.
     * The smooth flag is informational — the caller (PluginProcessor)
     * handles SmoothedValue ramp time adjustment before calling this.
     */
    void loadPreset (int index, bool smooth);

    /** Returns true if any dimension differs from the loaded preset by more than 0.5. */
    bool isDirty() const;

    /** Returns preset name, appending " *" if dirty. Empty string if invalid index. */
    juce::String getCurrentDisplayName() const;

    /** Writes factory preset XML files to disk if they don't already exist. */
    void initializePresetFiles();

    /** Returns the presets directory path. */
    static juce::File getPresetsDirectory();

private:
    juce::AudioProcessorValueTreeState& apvts;
    int currentIndex = 0;
};
