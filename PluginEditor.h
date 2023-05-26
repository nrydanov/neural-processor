#pragma once

#include "PluginProcessor.h"

//==============================================================================
class NeuralProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit NeuralProcessorEditor (NeuralProcessor&);
    ~NeuralProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NeuralProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralProcessorEditor)
};
