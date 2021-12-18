/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaterphonePluginAudioProcessorEditor::WaterphonePluginAudioProcessorEditor (WaterphonePluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), bellButton("Shape Button", juce::Colours::purple, juce::Colours::black, juce::Colours::blue), prevButtonState(0)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 400);
    
    float bellWidth = 5.0f;
    float centreBellHeight = getHeight()/2;
    juce::Path buttonShape;
    
    buttonShape.addRectangle(0, 0, bellWidth, centreBellHeight); //RECTANGLE position of button dictated in resized function below
    
    //buttonShape.addEllipse(0, 0, bellWidth, centreBellHeight); //CIRCLE
    
    bellButton.setShape(buttonShape, true, true, false);
    //addAndMakeVisible(&bellButton);
    
    bellButton.addListener(this);
    
    //BUTTON 2
    //juce::Path buttonShape2;
    //buttonShape2.addRectangle(0, 0, bellWidth, centreBellHeight);

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
    
    //POSITION OF SHAPE BUTTON (simple)
    //bellButton.setTopLeftPosition(getWidth()/2,getHeight()*0.03);
    
    //POSITION OF SHAPE BUTTONS
    auto xPos = getWidth()/2;
    auto yPos = getHeight()*0.03;
    auto yDistance = 50;
    //int test = 0;
    
    //int buttonRange[] = {0, 1, 2, 3, 4, 5};
    
    //for (auto bellButton : buttonRange)
    //{
        bellButton.setTopLeftPosition (xPos, yPos);
        yPos += yDistance; //defines distance between bells
        addAndMakeVisible(&bellButton);
        
        //test++;
        //juce::Logger::writeToLog("Loop Run " + std::to_string(test));
        //the loop is working and will print to terminal 6 times
        //comment out for loop to place only one button, cannot call functions inside of loop for some reason
    //}

    //NOTES FROM PLUCKEDSTRINGDEMO
    //for (auto stringLine : stringLines)
    //{
    //    stringLine->setTopLeftPosition (xPos, yPos);
    //    yPos += yDistance;
    //    addAndMakeVisible (stringLine);
    //}
    
}

void WaterphonePluginAudioProcessorEditor::buttonClicked (juce::Button *button)
{
    juce::Logger::writeToLog("Button Clicked!");
}

void WaterphonePluginAudioProcessorEditor::buttonStateChanged (juce::Button *button)
{
    juce::Logger::writeToLog("Button state: " + std::to_string(button->getState()));
    
    
    if (button->getState() == 2)
    {
        auto message = juce::MidiMessage::noteOn (1, 49, (juce::uint8) 100);
        message.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001 - 0.0);
        audioProcessor.getMidiMessageCollector().addMessageToQueue(message);
        prevButtonState = button->getState();
    }
    
    else if (button->getState() == 1 && prevButtonState == 2)
    {
        auto message = juce::MidiMessage::noteOff (1, 49, (juce::uint8) 100);
        message.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001 - 0.0);
        audioProcessor.getMidiMessageCollector().addMessageToQueue(message);
        prevButtonState = button->getState();
    }
    
}
