#include "PluginProcessor.h"

TonewordAudioProcessor::TonewordAudioProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

TonewordAudioProcessor::~TonewordAudioProcessor() {}

const juce::String TonewordAudioProcessor::getName() const { return "Toneword"; }
bool TonewordAudioProcessor::acceptsMidi() const { return false; }
bool TonewordAudioProcessor::producesMidi() const { return false; }
bool TonewordAudioProcessor::isMidiEffect() const { return false; }
double TonewordAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int TonewordAudioProcessor::getNumPrograms() { return 1; }
int TonewordAudioProcessor::getCurrentProgram() { return 0; }
void TonewordAudioProcessor::setCurrentProgram (int index) { juce::ignoreUnused (index); }
const juce::String TonewordAudioProcessor::getProgramName (int index) { juce::ignoreUnused (index); return {}; }
void TonewordAudioProcessor::changeProgramName (int index, const juce::String& newName) { juce::ignoreUnused (index, newName); }

void TonewordAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void TonewordAudioProcessor::releaseResources() {}

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

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Audio passes through unchanged — DSP engine wired in Plan 02/03
}

bool TonewordAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* TonewordAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

void TonewordAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

void TonewordAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TonewordAudioProcessor();
}
