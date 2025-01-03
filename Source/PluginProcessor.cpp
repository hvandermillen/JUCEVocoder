/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicVocoderAudioProcessor::BasicVocoderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                     #if ! JucePlugin_IsSynth
                       .withInput  ("SideChain",  juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), waveViewer(1),
        state (*this, nullptr, "STATE", {
            std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 1.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("mix", "Dry / Wet", 0.0f, 1.0f, 1.0f),
        })
#endif
{
    waveViewer.setRepaintRate(30);
    waveViewer.setBufferSize(256);
}

BasicVocoderAudioProcessor::~BasicVocoderAudioProcessor()
{
}

//==============================================================================
const juce::String BasicVocoderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicVocoderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicVocoderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicVocoderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicVocoderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicVocoderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicVocoderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicVocoderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicVocoderAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicVocoderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicVocoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    env = EnvelopeFollower();
    env.Init(20000,200000);
    
    bands[0].Init(sampleRate, 500);
    bands[1].Init(sampleRate, 500);
    
    vocoders[0].Init(sampleRate);
    vocoders[1].Init(sampleRate);
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BasicVocoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicVocoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicVocoderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto mainIO = getBusBuffer (buffer, true, 0);
    auto sidechain = getBusBuffer (buffer, true,1);
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    float gain = state.getParameter("gain")->getValue();
    float mix = state.getParameter("mix")->getValue();
   
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
    for (int channel = 0; channel < mainIO.getNumChannels(); ++channel)
    {
        auto* channelData = mainIO.getWritePointer (channel);
        //should be sidechain.getReadPointer but that is not working

        // ..do something to the data...
        
//        float whiteNoise = ((float)(rand()) / (float)(RAND_MAX)) * 2 - 1;
        
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            float carrierSample = 0;
            if (getTotalNumInputChannels() > 2)
                carrierSample = buffer.getSample(3,i);
            else
                carrierSample = ((float)(rand()) / (float)(RAND_MAX)) * 2 - 1;
            float drySignal = channelData[i];
            float wetSignal = vocoders[channel].Process(channelData[i],carrierSample);
            wetSignal *= 4;
            channelData[i] = ((wetSignal * mix) + (drySignal * (1.0f-mix))) * gain;
            
        }
    }
    waveViewer.pushBuffer(buffer);
}

float BasicVocoderAudioProcessor::getEnvLevel() {
    return env.getLevel();
}

//==============================================================================
bool BasicVocoderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicVocoderAudioProcessor::createEditor()
{
    return new BasicVocoderAudioProcessorEditor (*this);
}

//==============================================================================
void BasicVocoderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BasicVocoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicVocoderAudioProcessor();
}
