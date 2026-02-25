#include "PluginProcessor.h"
#include "PluginEditor.h"

TonewordAudioProcessor::TonewordAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, &undoManager, "PARAMETERS", createParameterLayout()),
      presetManager (parameters)
{
    // Cache atomic pointers to parameter values for lock-free audio-thread access
    for (int i = 0; i < NUM_DIMENSIONS; ++i)
    {
        auto* param = parameters.getRawParameterValue (ParamIDs::allIDs[i]);
        parameterValues[static_cast<size_t> (i)] = param;
    }

    bypassParam    = parameters.getRawParameterValue (ParamIDs::Bypass);
    snapSmoothParam = parameters.getRawParameterValue (ParamIDs::SnapSmooth);

    // Write factory preset XML files to disk if they don't exist yet
    presetManager.initializePresetFiles();
}

TonewordAudioProcessor::~TonewordAudioProcessor() {}

const juce::String TonewordAudioProcessor::getName() const { return "Toneword"; }
bool TonewordAudioProcessor::acceptsMidi() const { return false; }
bool TonewordAudioProcessor::producesMidi() const { return false; }
bool TonewordAudioProcessor::isMidiEffect() const { return false; }
double TonewordAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int TonewordAudioProcessor::getNumPrograms() { return presetManager.getNumPresets(); }
int TonewordAudioProcessor::getCurrentProgram() { return presetManager.getCurrentIndex(); }

void TonewordAudioProcessor::setCurrentProgram (int index)
{
    bool isSmooth = snapSmoothParam->load() > 0.5f;

    if (isSmooth)
    {
        // Temporarily extend ramp for smooth preset morph (~150ms)
        double sr = getSampleRate();
        if (sr > 0)
        {
            for (auto& sv : smoothedDimensions)
                sv.reset (sr, 0.15); // 150ms morph
        }
    }

    presetManager.loadPreset (index, isSmooth);

    if (isSmooth)
    {
        // Restore normal 20ms ramp — only affects the NEXT target change,
        // the current smoothing in progress will complete at 150ms rate
        double sr = getSampleRate();
        if (sr > 0)
        {
            for (auto& sv : smoothedDimensions)
                sv.reset (sr, 0.02); // 20ms ramp
        }
    }
}

const juce::String TonewordAudioProcessor::getProgramName (int index) { return presetManager.getPresetName (index); }
void TonewordAudioProcessor::changeProgramName (int index, const juce::String& newName) { juce::ignoreUnused (index, newName); }

juce::AudioProcessorParameter* TonewordAudioProcessor::getBypassParameter() const
{
    return parameters.getParameter (ParamIDs::Bypass);
}

void TonewordAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Prepare the DSP engine
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());

    semanticEQ.prepare (spec);

    // Reset SmoothedValues with 20ms ramp time
    for (auto& sv : smoothedDimensions)
    {
        sv.reset (sampleRate, 0.02); // 20ms ramp
        sv.setCurrentAndTargetValue (0.0f);
    }

    // Set initial SmoothedValue targets from current parameter values
    for (int i = 0; i < NUM_DIMENSIONS; ++i)
    {
        float val = parameterValues[static_cast<size_t> (i)]->load();
        smoothedDimensions[static_cast<size_t> (i)].setCurrentAndTargetValue (val);
    }
}

void TonewordAudioProcessor::releaseResources()
{
    semanticEQ.reset();
}

bool TonewordAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void TonewordAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any extra output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Skip DSP when bypassed — pass audio through unprocessed
    if (bypassParam->load() > 0.5f)
        return;

    // Read parameter values and update SmoothedValues
    bool anySmoothing = false;
    for (int i = 0; i < NUM_DIMENSIONS; ++i)
    {
        float target = parameterValues[static_cast<size_t> (i)]->load();
        smoothedDimensions[static_cast<size_t> (i)].setTargetValue (target);

        if (smoothedDimensions[static_cast<size_t> (i)].isSmoothing())
            anySmoothing = true;
    }

    // Skip the next value to advance the SmoothedValue,
    // then set the dimension on the EQ
    for (int i = 0; i < NUM_DIMENSIONS; ++i)
    {
        float smoothed = smoothedDimensions[static_cast<size_t> (i)].skip (buffer.getNumSamples());
        semanticEQ.setDimension (static_cast<Dimension> (i), smoothed);
    }

    juce::ignoreUnused (anySmoothing);

    // Process audio through the semantic EQ
    juce::dsp::AudioBlock<float> block (buffer);
    semanticEQ.process (block);

    // Push post-EQ samples to spectrum FIFO for visualization
    auto* channelData = buffer.getReadPointer (0);
    for (int i = 0; i < buffer.getNumSamples(); ++i)
        spectrumData.pushSample (channelData[i]);
}

bool TonewordAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* TonewordAudioProcessor::createEditor()
{
    return new TonewordAudioProcessorEditor (*this);
}

void TonewordAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    state.setProperty ("currentPreset", presetManager.getCurrentIndex(), nullptr);
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void TonewordAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName (parameters.state.getType()))
    {
        parameters.replaceState (juce::ValueTree::fromXml (*xml));
        int restoredIndex = parameters.state.getProperty ("currentPreset", 0);
        presetManager.setCurrentIndex (restoredIndex);
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TonewordAudioProcessor();
}
