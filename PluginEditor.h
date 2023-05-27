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
    void loadCheckpoint();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NeuralProcessor& processorRef;
    std::unique_ptr<juce::TextButton> button;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralProcessorEditor)
};
