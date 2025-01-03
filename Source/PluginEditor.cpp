/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicVocoderAudioProcessorEditor::BasicVocoderAudioProcessorEditor (BasicVocoderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(audioProcessor.waveViewer);
    audioProcessor.waveViewer.setColours(juce::Colours::black, juce::Colours::blueviolet.withAlpha(0.5f));
}

BasicVocoderAudioProcessorEditor::~BasicVocoderAudioProcessorEditor()
{
}

//==============================================================================
void BasicVocoderAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    juce::String text = "Hello World!";
    g.drawFittedText (std::to_string(audioProcessor.getTotalNumInputChannels()), getLocalBounds(), juce::Justification::centred, 1);
}

void BasicVocoderAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    audioProcessor.waveViewer.setBounds(getLocalBounds().withSizeKeepingCentre(getWidth(),getHeight()));
}
