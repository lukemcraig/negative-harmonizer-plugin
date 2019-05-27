/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
	std::vector<std::unique_ptr<AudioParameterFloat>> params;
	params.push_back(std::make_unique<AudioParameterFloat>("tonic", // parameter ID
		"Tonic", // parameter Name
		NormalisableRange < float > (0,11,1),
		0
		)
	);
	params.push_back(std::make_unique<AudioParameterFloat>("octave", // parameter ID
		"Octave", // parameter Name
		NormalisableRange < float >(-1, 9, 1),
		4
		)
	);
	return { params.begin(), params.end() };
}
//==============================================================================
NegativeHarmonizerPluginAudioProcessor::NegativeHarmonizerPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
	),
#endif
	parameters(*this, nullptr, Identifier("NegativeHarmonizer"), createParameterLayout())
{
	tonicParameter = parameters.getRawParameterValue("tonic");
	octaveParameter = parameters.getRawParameterValue("octave");
}

NegativeHarmonizerPluginAudioProcessor::~NegativeHarmonizerPluginAudioProcessor()
{
}

//==============================================================================
const String NegativeHarmonizerPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NegativeHarmonizerPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NegativeHarmonizerPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NegativeHarmonizerPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NegativeHarmonizerPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NegativeHarmonizerPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NegativeHarmonizerPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NegativeHarmonizerPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String NegativeHarmonizerPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void NegativeHarmonizerPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NegativeHarmonizerPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NegativeHarmonizerPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NegativeHarmonizerPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void NegativeHarmonizerPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
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

	MidiBuffer processedMidi;
	int time;
	MidiMessage m;
	for (MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
	{
		if (m.isNoteOn() || m.isNoteOff())
		{
            const auto oldNote = m.getNoteNumber();
            const auto originalNoteDistance = mirrorAxis(*tonicParameter, *octaveParameter) - oldNote;
            const auto newNote = mirrorAxis(*tonicParameter, *octaveParameter) + originalNoteDistance;
			if (newNote > 0 && newNote < 128) {
				if (m.isNoteOn()) {
					m = MidiMessage::noteOn(m.getChannel(), static_cast<int>(newNote), m.getVelocity());
				}
				else
				{
					m = MidiMessage::noteOff(m.getChannel(), static_cast<int>(newNote), m.getVelocity());
				}
				processedMidi.addEvent(m, time);
			}
		}
		else
		{
			processedMidi.addEvent(m, time);
		}
		
	}
	midiMessages.swapWith(processedMidi);
}

//==============================================================================
bool NegativeHarmonizerPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NegativeHarmonizerPluginAudioProcessor::createEditor()
{
    return new NegativeHarmonizerPluginAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void NegativeHarmonizerPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	auto state = parameters.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void NegativeHarmonizerPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(parameters.state.getType()))
			parameters.replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NegativeHarmonizerPluginAudioProcessor();
}
