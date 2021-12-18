/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class WaterphonePluginAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, public juce::Slider::Listener
{
public:
    WaterphonePluginAudioProcessorEditor (WaterphonePluginAudioProcessor&);
    ~WaterphonePluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (juce::Slider *slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WaterphonePluginAudioProcessor& audioProcessor;
    
    //BELL BUTTONS
    juce::ShapeButton bellButton;
    
    void buttonClicked (juce::Button *button) override;
    
    void buttonStateChanged (juce::Button *button) override;
    
    int prevButtonState;
    
    //POTENTIOMETERS
    juce::Slider airPot;
    
    juce::Slider waterPot;
    
    juce::Slider dissonancePot;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterphonePluginAudioProcessorEditor)
};
