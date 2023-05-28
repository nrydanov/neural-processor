#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "RTNeural/RTNeural/RTNeural.h"
//==============================================================================
NeuralProcessor::NeuralProcessor()
     : AudioProcessor (BusesProperties()
    #if ! JucePlugin_IsMidiEffect
        #if ! JucePlugin_IsSynth
            .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        #endif
            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
        #endif
        ),
    parameters(*this, nullptr, juce::Identifier("Parameters"), {
        std::make_unique<juce::AudioParameterChoice>("modelType", "Model type", 
            juce::StringArray {"LightModel", "MediumModel", "HeavyModel"}, 0)})
{

}
NeuralProcessor::~NeuralProcessor()
{

}

//==============================================================================
const juce::String NeuralProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NeuralProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NeuralProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NeuralProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NeuralProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NeuralProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NeuralProcessor::getCurrentProgram()
{
    return 0;
}

void NeuralProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String NeuralProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void NeuralProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void NeuralProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void NeuralProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool NeuralProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

template<typename T>
void NeuralProcessor::processAbstractBlock (juce::AudioBuffer<T>& buffer,
                                                juce::MidiBuffer& midiMessages)
{
    
    juce::ignoreUnused(midiMessages);

    using namespace juce;
    ScopedNoDenormals noDenormals;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        T* outBuffer = buffer.getWritePointer(channel);
        auto numSamples = buffer.getNumSamples();
        std::visit([outBuffer, numSamples] (auto&& chosenModel) {
            for (int i = 0; i < numSamples; ++i) {
                float input[] = { outBuffer[i] };
                outBuffer[i] += chosenModel.forward(input);
            }
        }, model);
    }
}

void NeuralProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                       juce::MidiBuffer& midiMessages) {
    processAbstractBlock<float>(buffer, midiMessages);
}


//==============================================================================
bool NeuralProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NeuralProcessor::createEditor()
{
    return new NeuralProcessorEditor (*this);
}

//==============================================================================
void NeuralProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

void NeuralProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeuralProcessor();
}
