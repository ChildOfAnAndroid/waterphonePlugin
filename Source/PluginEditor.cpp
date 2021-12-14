/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaterphonePluginAudioProcessorEditor::WaterphonePluginAudioProcessorEditor (WaterphonePluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), shapeButton("Shape Button", juce::Colours::purple, juce::Colours::black, juce::Colours::blue)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    float buttonSize = 50.0f;
    juce::Path buttonShape;
    //buttonShape.addRectangle(0, 0, buttonSize, buttonSize); //RECTANGLE position of button (0, 0) center of screen
    buttonShape.addEllipse(0, 0, buttonSize, buttonSize); //CIRCLE
    
    shapeButton.setShape(buttonShape, true, true, false);
    addAndMakeVisible(&shapeButton);
    
    shapeButton.addListener(this);
}

WaterphonePluginAudioProcessorEditor::~WaterphonePluginAudioProcessorEditor()
{
}

//==============================================================================
void WaterphonePluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void WaterphonePluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    shapeButton.setTopLeftPosition(getWidth() / 2, getHeight() / 2); //centers the shape button
    
}

//void WaterphonePluginAudioProcessorEditor::buttonClicked (juce::Button *button)
//{
//    juce::Logger::writeToLog("Button Clicked!");
//}

void WaterphonePluginAudioProcessorEditor::buttonStateChanged (juce::Button *button)
{
    juce::Logger::writeToLog("Button State: " + std::to_string(button->getState()));
}
