#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NeuralProcessorEditor::NeuralProcessorEditor (NeuralProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    this->button = std::make_unique<juce::TextButton>("Click me!");
    this->button->onClick = [this] {loadCheckpoint();};
}

NeuralProcessorEditor::~NeuralProcessorEditor()
{
}

void NeuralProcessorEditor::loadCheckpoint()
{
    using namespace juce;    
    auto chooser = std::make_unique<FileChooser>("Please select the checkpoint",
            File::getSpecialLocation (File::userHomeDirectory), "*.json");
    
    auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories;
    chooser->launchAsync (folderChooserFlags, [this] (const FileChooser& chooser) {
            auto path = chooser.getResult().getFullPathName();
            
            std::ifstream jsonStream(path.toStdString(), std::ifstream::binary);
            auto model = RTNeural::json_parser::parseJson<double>(jsonStream); 
            model->reset();
            processorRef.model = std::move(model);
    });
}

//==============================================================================
void NeuralProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    this->button->paintButton(g, true, false);
}

void NeuralProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
