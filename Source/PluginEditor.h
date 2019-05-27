/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NegativeHarmonizerPluginAudioProcessorEditor  : public AudioProcessorEditor
{
public:    
    NegativeHarmonizerPluginAudioProcessorEditor (NegativeHarmonizerPluginAudioProcessor&, AudioProcessorValueTreeState&);
    ~NegativeHarmonizerPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NegativeHarmonizerPluginAudioProcessor& processor;

	AudioProcessorValueTreeState& valueTreeState;

	Label tonicLabel;
	Slider tonicSlider;
	std::unique_ptr<SliderAttachment> tonicAttachment;

	Label octaveLabel;
	Slider octaveSlider;
	std::unique_ptr<SliderAttachment> octaveAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NegativeHarmonizerPluginAudioProcessorEditor)    
};
