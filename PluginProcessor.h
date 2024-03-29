#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <string>
#include <memory>

#include "RTNeural/RTNeural/RTNeural.h"
//==============================================================================
class NeuralProcessor : public juce::AudioProcessor
{
public:
    using LightModel = RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 6>, 
            RTNeural::DenseT<float, 6, 1>>;
    using MediumModel = RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 16>,
          RTNeural::DenseT<float, 16, 1>>;
    using HeavyModel = RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 32>,
          RTNeural::DenseT<float, 32, 1>>;
    using ModelVariant = std::variant<LightModel, MediumModel, HeavyModel>;
    //==============================================================================
    NeuralProcessor();
    NeuralProcessor(juce::String jsonPath);
    ~NeuralProcessor() override;
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    template<typename T>
    void processAbstractBlock(juce::AudioBuffer<T>&, juce::MidiBuffer&);

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    friend class NeuralProcessorEditor;

    bool loaded = false;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralProcessor)   
    
    ModelVariant model;
    juce::AudioProcessorValueTreeState parameters;
};
