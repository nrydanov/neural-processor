#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeuralProcessorEditor::NeuralProcessorEditor (NeuralProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    this->button = std::make_unique<juce::TextButton>();
    this->button->setButtonText("Click me!");
    this->button->setColour(juce::TextButton::buttonColourId, juce::Colours::lime);
    this->button->onClick = [this] {loadCheckpoint();};

    this->chooser = std::make_unique<juce::FileChooser>("Please select the checkpoint",
            juce::File::getSpecialLocation (juce::File::userHomeDirectory), "*.json", false);
    addAndMakeVisible(*this->button);
    setSize (800, 600);
}

NeuralProcessorEditor::~NeuralProcessorEditor()
{
}

void NeuralProcessorEditor::loadCheckpoint()
{   
    using namespace juce;    
    
    auto folderChooserFlags = FileBrowserComponent::openMode 
        | FileBrowserComponent::canSelectDirectories
        | FileBrowserComponent::canSelectFiles;
    this->chooser->launchAsync (folderChooserFlags, [this] (const FileChooser& c) {
            using ModelType = NeuralProcessor::ModelType;

            auto path = c.getResult().getFullPathName();
            std::ifstream jsonStream(path.toStdString(), std::ifstream::binary);
            nlohmann::json modelJson;
            jsonStream >> modelJson;
            processorRef.model = std::make_unique<ModelType>();
            RTNeural::torch_helpers::loadLSTM<float>(modelJson, "lstm.", processorRef.model->get<0>());
            RTNeural::torch_helpers::loadDense<float>(modelJson, "linear.", processorRef.model->get<1>());
            processorRef.model->reset();
    });
}

//==============================================================================
void NeuralProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void NeuralProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    this->button->setBounds(getLocalBounds());
}

