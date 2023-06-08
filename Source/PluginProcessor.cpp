/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "FaderValueTree.h"
#include "PluginEditor.h"

//==============================================================================
Fader_RiderAudioProcessor::Fader_RiderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	m_pValueTreeState = std::make_unique<FaderValueTree>(*this);

}

Fader_RiderAudioProcessor::~Fader_RiderAudioProcessor()
{
}

//==============================================================================
const juce::String Fader_RiderAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool Fader_RiderAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool Fader_RiderAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool Fader_RiderAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double Fader_RiderAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int Fader_RiderAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int Fader_RiderAudioProcessor::getCurrentProgram()
{
	return 0;
}

void Fader_RiderAudioProcessor::setCurrentProgram(int /*index*/)
{
}

const juce::String Fader_RiderAudioProcessor::getProgramName(int /*index*/)
{
	return {};
}

void Fader_RiderAudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/)
{
}

//==============================================================================
void Fader_RiderAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
	juce::dsp::ProcessSpec spec;

	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = 1;

	leftChannel.prepare(spec);
	rightChannel.prepare(spec);
}

void Fader_RiderAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Fader_RiderAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
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

void Fader_RiderAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
	juce::ScopedNoDenormals noDenormals;
	const auto totalNumInputChannels = getTotalNumInputChannels();
	const auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.
	float averageGain{ 0.f };
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		const float level = buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
		averageGain += juce::Decibels::gainToDecibels(level);
	}
	averageGain /= totalNumInputChannels;
	m_pValueTreeState->SetGainLevel(averageGain);

	juce::dsp::AudioBlock<float> block(buffer);
	auto leftBlock = block.getSingleChannelBlock(0);
	auto rightBlock = block.getSingleChannelBlock(1);

	const juce::dsp::ProcessContextReplacing leftContext(leftBlock);
	const juce::dsp::ProcessContextReplacing rightContext(rightBlock);

	leftChannel.process(leftContext);
	rightChannel.process(rightContext);

	const float faderLevel = m_pValueTreeState->getRawParameterValue("FaderLevel")->load();
	const auto leftGain = faderLevel + leftChannel.get<1>().getGainDecibels();
	const auto rightGain = faderLevel + rightChannel.get<1>().getGainDecibels();

	leftChannel.get<1>().setGainDecibels(leftGain);
	rightChannel.get<1>().setGainDecibels(rightGain);
}

//==============================================================================
bool Fader_RiderAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Fader_RiderAudioProcessor::createEditor()
{
	//return new Fader_RiderAudioProcessorEditor(*this);
	return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void Fader_RiderAudioProcessor::getStateInformation(juce::MemoryBlock& /*destData*/)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void Fader_RiderAudioProcessor::setStateInformation(const void* /*data*/, int /*sizeInBytes*/)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new Fader_RiderAudioProcessor();
}
