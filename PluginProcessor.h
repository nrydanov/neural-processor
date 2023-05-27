#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <string>
#include <memory>

#include "RTNeural/RTNeural/RTNeural.h"
//==============================================================================
class NeuralProcessor : public juce::AudioProcessor
{
public:
    using ModelType = RTNeural::ModelT<double, 1, 1, RTNeural::LSTMLayerT<double, 1, 64>, 
            RTNeural::DenseT<double, 64, 1>>;
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
    void processBlock (juce::AudioBuffer<double>&, juce::MidiBuffer&) override;

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

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralProcessor)   
    
    std::unique_ptr<ModelType> model;

};
