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
    //juce::Path buttonShape;
    //buttonShape.addRectangle(0, 0, bellWidth, centreBellHeight);
    
    //POTENTIOMETERS
    
    //air pot
    addAndMakeVisible (&airPot);
    airPot.setSliderStyle (juce::Slider::Rotary);
    airPot.setRange (0.0f, 12.0f, 0.01f); //Bottom of the pot is 0, top of the pot is 12
    airPot.setValue (0.0f);
    airPot.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 150, 25);
    airPot.setTextValueSuffix (" AIR");
    //airPot.setSkewFactor(2);
    airPot.addListener (this);
    
    //water pot
    addAndMakeVisible (&waterPot);
    waterPot.setSliderStyle (juce::Slider::Rotary);
    waterPot.setRange (0.0f, 12.0f, 0.01f); //Bottom of the pot is 0, top of the pot is 12
    waterPot.setValue (0.0f);
    waterPot.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 150, 25);
    waterPot.setTextValueSuffix (" WATER");
    //waterPot.setSkewFactor(2);
    waterPot.addListener (this);
    
    //dissonance pot
    addAndMakeVisible (&dissonancePot);
    dissonancePot.setSliderStyle (juce::Slider::Rotary);
    dissonancePot.setRange (0.0f, 12.0f, 0.01f); //Bottom of the pot is 0, top of the pot is 12
    dissonancePot.setValue (0.0f);
    dissonancePot.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 150, 25);
    dissonancePot.setTextValueSuffix (" DISSONANCE");
    //dissonancePot.setSkewFactor(2);
    dissonancePot.addListener (this);

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
    g.drawFittedText ("Waterphone Plugin", getLocalBounds(), juce::Justification::topLeft, 1);
    
    //BUTTON ANIMATION ATTEMPT (currently just makes line)
    auto xPos = getWidth()/2;
    auto yPos = getHeight()*0.03;
    float bellWidth = 5.0f;
    float centreBellHeight = getHeight()/2;
    
    juce::Path bellPath;
    
    bellPath.startNewSubPath (xPos, yPos);
    //bellPath.lineTo(xPos, yPos+centreBellHeight);
    bellPath.quadraticTo(xPos, yPos, xPos, yPos+centreBellHeight);
    
    g.strokePath (bellPath, juce::PathStrokeType (2.0f));
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
    
    //POTENTIOMETER POSITIONING
    airPot.setBounds (75, getHeight() / 2 + 30, 150, 150);
    waterPot.setBounds (425, getHeight() / 2 + 30, 150, 150);
    dissonancePot.setBounds (775, getHeight() / 2 + 30, 150, 150);
    
    //airPot.setBounds(<#int x#>, <#int y#>, <#int width#>, <#int height#>)
    
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

void WaterphonePluginAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    
    if (slider == &airPot)
    {
        audioProcessor.airPotAmount = airPot.getValue();
    }
    
    else if (slider == &waterPot)
    {
        audioProcessor.waterPotAmount = waterPot.getValue();
    }
    
    else if (slider == &dissonancePot)
    {
        audioProcessor.dissonancePotAmount = dissonancePot.getValue();
    }
    
}
