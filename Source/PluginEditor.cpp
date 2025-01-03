/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicVocoderAudioProcessorEditor::BasicVocoderAudioProcessorEditor (BasicVocoderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
                gainAttachment     (p.state, "gain",    gainSlider),
                mixAttachment      (p.state, "mix",     mixSlider)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(audioProcessor.waveViewer);
    audioProcessor.waveViewer.setColours(juce::Colours::black, juce::Colours::blueviolet.withAlpha(0.5f));
    
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(mixSlider);
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,60,15);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,false,60,15);
    
    int border = 30;
    int dialWidth = getWidth() / 4 - border;
    int dialHeight = getHeight() / 2 - border - 30;
    
    gainSlider.setBounds(border, getHeight()-border-100, dialWidth, dialHeight);
    mixSlider.setBounds(getWidth()-dialWidth-border, getHeight()-border-100, dialWidth, dialHeight);
    gainTextArea.setBounds(gainSlider.getX()-dialWidth/4,gainSlider.getY()-10, dialWidth, 25);
    mixTextArea.setBounds(mixSlider.getX()-dialWidth/4,mixSlider.getY()-10, dialWidth, 25);

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
    juce::String text = "Henry's Vocoder";
    g.drawFittedText (text, getLocalBounds(), juce::Justification::centred, 1);
    g.drawFittedText ("Gain", gainTextArea, juce::Justification::centred, 1);
    g.drawFittedText ("Mix", mixTextArea, juce::Justification::centred, 1);
}

void BasicVocoderAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    audioProcessor.waveViewer.setBounds(getLocalBounds().withSizeKeepingCentre(getWidth(),getHeight()*0.4));
    
    audioProcessor.waveViewer.setBounds(0,0,getWidth(),getHeight()*0.4);
    
    int border = 30;
    int dialWidth = getWidth() / 4 - border;
    int dialHeight = getHeight() / 2 - border - 30;
    
    gainTextArea.setBounds(gainSlider.getX()-gainSlider.getWidth()/2,gainSlider.getY()-50, 100, 25);
    mixTextArea.setBounds(mixSlider.getX()-mixSlider.getWidth()/2,mixSlider.getY()-50, 100, 25);
}
