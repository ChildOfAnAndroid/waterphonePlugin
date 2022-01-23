/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class WaterphonePluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    WaterphonePluginAudioProcessor();
    ~WaterphonePluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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

    
    juce::MidiMessageCollector& getMidiMessageCollector() noexcept {return midiMessageCollector; };
    
    //These are the values that are set by the sliders in the GUI
    float airPotAmount { 0.5f };
    float waterPotAmount { 0.5f };
    
    //SENDING DISSONANCE SLIDER VALUE TO SINEWAVEVOICE CLASS
    float dissonancePotAmount { 0.5f };
    
    float attack { 0.5f };
    //float decay { 0.476f };
    //float sustain { 0.5f };
    //float release { 12.8f };
    
    void updateSounds ();
    

    
    //void setDissonancePotAmount(int dissonanceSlider)
    //{
    //    dissonancePotAmount = dissonanceSlider;
    //}
    //int getDissonancePotAmount()
    //{
    //    return dissonancePotAmount;
    //}

private:
    //==============================================================================
    
    juce::MidiMessageCollector midiMessageCollector;
    //juce::Synthesiser synth;
    juce::AudioFormatManager formatManager;
    
    //SOUND PROGRAMMING
    
    juce::Synthesiser sinSynth;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaterphonePluginAudioProcessor)
};

//SINE WAVE SOUND CLASS
struct SineWaveSound : public juce::SynthesiserSound
{
    SineWaveSound() { };
    
    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;
};
// Go to PluginProcessor.cpp to define the functions

//SINE WAVE VOICE CLASS
struct SineWaveVoice : public juce::SynthesiserVoice
{
    SineWaveVoice();
    SineWaveVoice(float dissonanceSliderValue, float attack);
    
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
        
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    
    //GETTING DISSONANCE SLIDER VALUE FROM WATERPHONEPLUGINAUDIOPROCESSOR CLASS

    //float dissonanceSliderValue = dissonancePotAmount;
    float dissonanceSliderValue { 0.5f };
    
    //void setDissonanceSliderValue(int dissonanceSlider)
    //{
    //    dissonanceSliderValue = dissonanceSlider;
    //}
    //int getDissonanceSliderValue()
    //{
    //    return dissonanceSliderValue;
    //}
    

    
private:
    double currentAngle, angleDelta, level, tailOff;
    
    //ADSR
    juce::ADSR adsr;
    
    float attack;
    
    //juce::ADSR::Parameters adsrParams;
    
    bool isPrepared {false} ;
    
};
//Go to pluginprocessor.cpp to define the functions
