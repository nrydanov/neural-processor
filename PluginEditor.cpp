#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeuralProcessorEditor::NeuralProcessorEditor (NeuralProcessor& p)
    : GenericAudioProcessorEditor (p), processorRef (p)
{
    this->button = std::make_unique<juce::TextButton>();
    this->button->setButtonText("Load checkpoint");
    this->button->onClick = [this] {loadCheckpoint();};

    this->chooser = std::make_unique<juce::FileChooser>("Please select the checkpoint",
            juce::File::getSpecialLocation (juce::File::userHomeDirectory), "*.json", true);
    addAndMakeVisible(*this->button);
    setSize (800, 600);
}

NeuralProcessorEditor::~NeuralProcessorEditor()
{
}

void NeuralProcessorEditor::loadModel(const nlohmann::json& modelJson) 
{
    juce::String modelType = processorRef.parameters
            .getParameter("modelType")->getCurrentValueAsText();
    if (modelType == "LightModel") {
        processorRef.model.emplace<NeuralProcessor::LightModel>();
    }
    if (modelType == "MediumModel") {
        processorRef.model.emplace<NeuralProcessor::MediumModel>();
    }
    if (modelType == "HeavyModel") {
        processorRef.model.emplace<NeuralProcessor::HeavyModel>();
    }
    std::visit([modelJson] (auto&& chosenModel) {
        RTNeural::torch_helpers::loadLSTM<float>(modelJson, "lstm.", 
                chosenModel.template get<0>());
        RTNeural::torch_helpers::loadDense<float>(modelJson, "linear.", 
                chosenModel.template get<1>());
    },processorRef.model);
}

void NeuralProcessorEditor::loadCheckpoint()
{   
    using namespace juce;  
    
    auto folderChooserFlags = FileBrowserComponent::openMode 
        | FileBrowserComponent::canSelectDirectories
        | FileBrowserComponent::canSelectFiles;
    this->chooser->launchAsync (folderChooserFlags, [this] (const FileChooser& c) {
            auto path = c.getResult().getFullPathName();
            if (path == "") {
                return;
            }
            std::ifstream jsonStream(path.toStdString(), std::ifstream::binary);
            nlohmann::json modelJson;
            jsonStream >> modelJson;

            loadModel(modelJson);
    });
}

//==============================================================================
void NeuralProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void NeuralProcessorEditor::resized()
{
    float frac = 0.2;
    int width = frac * getWidth();
    int height = frac * getHeight();
    int x = getX() + getWidth() / 2 - width / 2;
    int y = getY() + getHeight() - height;
    this->button->setBounds(x, y, width, height);
}

