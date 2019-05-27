/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NegativeHarmonizerPluginAudioProcessorEditor::NegativeHarmonizerPluginAudioProcessorEditor (NegativeHarmonizerPluginAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState(vts)
{
	tonicSlider.setSliderStyle(Slider::IncDecButtons);
	addAndMakeVisible(tonicSlider);
	tonicAttachment.reset(new SliderAttachment(valueTreeState, "tonic", tonicSlider));
	
    addAndMakeVisible(tonicLabel);
	tonicLabel.setText("Tonic", dontSendNotification);
	tonicLabel.attachToComponent(&tonicSlider, true);


	octaveSlider.setSliderStyle(Slider::IncDecButtons);
	addAndMakeVisible(octaveSlider);
	octaveAttachment.reset(new SliderAttachment(valueTreeState, "octave", octaveSlider));

	addAndMakeVisible(octaveLabel);
	octaveLabel.setText("Octave", dontSendNotification);
	octaveLabel.attachToComponent(&octaveSlider, true);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

NegativeHarmonizerPluginAudioProcessorEditor::~NegativeHarmonizerPluginAudioProcessorEditor()
{
}

//==============================================================================
void NegativeHarmonizerPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Negative Harmonizer - Luke M. Craig - " __DATE__ + String(" ") + __TIME__, getLocalBounds(), Justification::centredTop, 1);
}

void NegativeHarmonizerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	tonicSlider.setBounds(40, 30, getWidth()-60, 40);
	octaveSlider.setBounds(40, 80, getWidth() - 60, 40);
}

