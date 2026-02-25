#pragma once

#include "DSP/SemanticEQ.h"
#include "Parameters.h"
#include "Presets/PresetManager.h"
#include <juce_audio_processors/juce_audio_processors.h>

class TonewordAudioProcessor : public juce::AudioProcessor
{
public:
    TonewordAudioProcessor();
    ~TonewordAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorParameter* getBypassParameter() const override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return parameters; }
    PresetManager& getPresetManager() { return presetManager; }

private:
    juce::UndoManager undoManager;
    juce::AudioProcessorValueTreeState parameters;

    // DSP engine
    SemanticEQ semanticEQ;

    // SmoothedValue for each dimension — prevents zipper noise
    std::array<juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear>, NUM_DIMENSIONS> smoothedDimensions;

    // Atomic pointers to parameter values (lock-free read from audio thread)
    std::array<std::atomic<float>*, NUM_DIMENSIONS> parameterValues {};

    std::atomic<float>* bypassParam = nullptr;
    std::atomic<float>* snapSmoothParam = nullptr;

    PresetManager presetManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TonewordAudioProcessor)
};
