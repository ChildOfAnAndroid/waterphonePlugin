/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaterphonePluginAudioProcessor::WaterphonePluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
    formatManager.registerBasicFormats();
    
    //for (auto i = 0; i < 5; i++)
        //synth.addVoice(new juce::SamplerVoice());
    
    //Load Sounds
    ///Users/cmale/Documents/GitHub/Degree-Tasks/CAMT503/Week 5/KeyboardSynthesiser/Sound Samples/piano-A3.wav
    
    //synth.clearSounds();
    //juce::File soundSample("/Users/charis/Documents/GitHub/Degree-Tasks/CAMT503/Week 5/KeyboardSynthesiser/Sound Samples/piano-A3.wav");
    //std::unique_ptr<juce::AudioFormatReader>
    //audioReader(formatManager.createReaderFor(soundSample));
    
    //juce::BigInteger allNotes;
    //allNotes.setRange(0, 128, true);
    
    //synth.addSound(new juce::SamplerSound("Piano",
                                          //*audioReader,
                                          //allNotes,
                                          //49,
                                          //0.01,
                                          //0.01,
                                          //10.0));
    
    //SOUND PROGRAMMING
    //add voices and sound to the sinSynth object
    for (auto i = 0; i < 5; i++) //creates polyphony of 5
    {
        sinSynth.addVoice(new SineWaveVoice());
    }
    
    sinSynth.addSound(new SineWaveSound());
}

WaterphonePluginAudioProcessor::~WaterphonePluginAudioProcessor()
{
}

//==============================================================================
const juce::String WaterphonePluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaterphonePluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaterphonePluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaterphonePluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaterphonePluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaterphonePluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaterphonePluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaterphonePluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WaterphonePluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaterphonePluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WaterphonePluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //synth.setCurrentPlaybackSampleRate(sampleRate);
    midiMessageCollector.reset(sampleRate);
    
    //SOUND PROGRAMMING
    
    sinSynth.setCurrentPlaybackSampleRate(sampleRate);
    
}

void WaterphonePluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaterphonePluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WaterphonePluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    
    for (int i = 0; i < sinSynth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(sinSynth.getVoice(i)))
        {
            //OSC Controls
            //ADSR
            //LFO
        }
    }
    
    midiMessageCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    
    auto audioBusBuffer = getBusBuffer(buffer, false, 0);
    
    //synth.renderNextBlock(audioBusBuffer, midiMessages, 0, audioBusBuffer.getNumSamples());
    sinSynth.renderNextBlock(audioBusBuffer, midiMessages, 0, audioBusBuffer.getNumSamples());


}

//==============================================================================
bool WaterphonePluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WaterphonePluginAudioProcessor::createEditor()
{
    return new WaterphonePluginAudioProcessorEditor (*this);
}

//==============================================================================
void WaterphonePluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaterphonePluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void WaterphonePluginAudioProcessor::updateSounds()
{
    sinSynth.clearSounds();
    sinSynth.clearVoices();
    
    SineWaveVoice(dissonancePotAmount, attack);
    SineWaveSound();
    
    for (auto i = 0; i < 5; i++)
    {
        sinSynth.addVoice(new SineWaveVoice(dissonancePotAmount, attack));
    }
    
    sinSynth.addSound(new SineWaveSound());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaterphonePluginAudioProcessor();
}

//SOUND PROGRAMMING
bool SineWaveSound::appliesToNote (int midiNoteNumber)
{
    return true;
}

bool SineWaveSound::appliesToChannel (int midiChannel)
{
    return true;
}

SineWaveVoice::SineWaveVoice() : currentAngle(0.0), angleDelta(0.0), level(0.0), tailOff(0.0)
{

}

SineWaveVoice::SineWaveVoice(float dissonanceSliderValue, float attack) : currentAngle(0.0), angleDelta(0.0), level(0.0), tailOff(0.0)
{
    SineWaveVoice::dissonanceSliderValue = dissonanceSliderValue;
    juce::Logger::writeToLog("Dissonance Slider Value: " + std::to_string(dissonanceSliderValue));
    
    SineWaveVoice::attack = attack;
    adsr.setSampleRate(getSampleRate());
    
    adsr.setParameters(juce::ADSR::Parameters(attack, 0.476, 0.5, 12.8));
}


bool SineWaveVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<SineWaveSound*>(sound) != nullptr;
}

void SineWaveVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)

{
    currentAngle = 0.0;
    level = velocity * 0.25;
    tailOff = 0.0;
    
    auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample = cyclesPerSecond / getSampleRate();  //GENERATES PITCH FOR SIN WAVE
    
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    
    //ADSR
    adsr.noteOn();
}

void SineWaveVoice::stopNote (float velocity, bool allowTailOff)
{
    if (allowTailOff)
    {
        if (tailOff == 0.0)
        {
            tailOff = 1.0;
        }
    }
    
    else
    {
        clearCurrentNote();
        angleDelta = 0.0;
    }
    
    //ADSR
    adsr.noteOff();
}

void SineWaveVoice::pitchWheelMoved (int newPitchWheelValue)
{

}

void SineWaveVoice::controllerMoved(int controllerNumber, int newCotrollerValue)
{

}

void SineWaveVoice::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    //ADSR
    //adsr.setSampleRate(sampleRate);
    //adsr.setParameters(adsrParams);
    
    isPrepared = true;
    
}


void SineWaveVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared = true);
    
    if (angleDelta != 0.0)
    {
        if (tailOff > 0.0)
        {
            while (--numSamples >= 0)
            {
                auto currentSample = (float)((std::sin(currentAngle)
                                              + std::sin((0.98 * (dissonanceSliderValue)) * currentAngle)
                                              + std::sin((0.99 * (dissonanceSliderValue)) * currentAngle)
                                              + std::sin((1.01 * (dissonanceSliderValue)) * currentAngle)
                                              + std::sin((1.02 * (dissonanceSliderValue)) * currentAngle)
                                              + std::sin((1.03 * (dissonanceSliderValue)) * currentAngle)
                                              * level));
                
                for (auto i = outputBuffer.getNumChannels();--i >=0;)
                {
                    outputBuffer.addSample(i, startSample, currentSample);
                }
                
                currentAngle += angleDelta;
                ++startSample;
                
                tailOff = tailOff * 0.99;
                
                if (tailOff <= 0.005)
                {
                    clearCurrentNote();
                    angleDelta = 0.0;
                    break;
                }
            }
        }
        
        else
        {
            while (--numSamples >= 0)
            {
                auto currentSample = (float)((std::sin(currentAngle)
                                            + std::sin((0.98 * (dissonanceSliderValue)) * currentAngle)
                                            + std::sin((0.99 * (dissonanceSliderValue)) * currentAngle)
                                            + std::sin((1.01 * (dissonanceSliderValue)) * currentAngle)
                                            + std::sin((1.02 * (dissonanceSliderValue)) * currentAngle)
                                            + std::sin((1.03 * (dissonanceSliderValue)) * currentAngle)
                                            * level));
                
                for (auto i = outputBuffer.getNumChannels();--i >=0;)
                {
                    outputBuffer.addSample(i, startSample, currentSample);
                }
                
                currentAngle += angleDelta;
                ++startSample;
                
            }
        }
    }

    //ADSR
    //adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
    
}
