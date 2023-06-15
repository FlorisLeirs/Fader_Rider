/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "FaderValueTree.h"

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
	m_pValueTreeState->UpdateParameterSettings();
	//m_pValueTreeState->SetBlockRate(static_cast<float>(sampleRate / samplesPerBlock));

	juce::dsp::ProcessSpec spec{};

	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = 1;

	m_LeftChannel.prepare(spec);
	m_RightChannel.prepare(spec);

	m_RightChannel.get<0>().setRatio(10.f);
	m_LeftChannel.get<0>().setRatio(10.f);

	m_RightChannel.get<0>().setAttack(0.f);
	m_LeftChannel.get<0>().setAttack(0.f);
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

	m_pValueTreeState->UpdateParameterSettings();

	// Noise gate
	const ParameterSettings params = m_pValueTreeState->GetParameterSettings();
	m_RightChannel.get<0>().setThreshold(params.VocalSensitivity);
	m_LeftChannel.get<0>().setThreshold(params.VocalSensitivity);

	UpdateGain(buffer, totalNumInputChannels);

	// DSP processing
	const juce::dsp::AudioBlock<float> block(buffer);
	auto leftBlock = block.getSingleChannelBlock(0);
	auto rightBlock = block.getSingleChannelBlock(1);

	const juce::dsp::ProcessContextReplacing leftContext(leftBlock);
	const juce::dsp::ProcessContextReplacing rightContext(rightBlock);

	m_LeftChannel.process(leftContext);
	m_RightChannel.process(rightContext);
}

//==============================================================================
bool Fader_RiderAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Fader_RiderAudioProcessor::createEditor()
{
	return new Fader_RiderAudioProcessorEditor(*this);
	//return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void Fader_RiderAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	juce::MemoryOutputStream mos(destData, true);
	m_pValueTreeState->state.writeToStream(mos);
}

void Fader_RiderAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	auto newTree = juce::ValueTree::readFromData(data, sizeInBytes);
	if(newTree.isValid())
	{
		m_pValueTreeState->replaceState(newTree);
		m_pValueTreeState->UpdateParameterSettings();
	}

}

void Fader_RiderAudioProcessor::UpdateGain(juce::AudioBuffer<float>& buffer, int numInputChannels)
{
	float averageGain{ 0.f };
	for (int channel = 0; channel < numInputChannels; ++channel)
	{
		const float level = buffer.getRMSLevel(channel, 0, buffer.getNumSamples());
		averageGain += juce::Decibels::gainToDecibels(level);
	}
	averageGain /= numInputChannels;
	m_RMS = averageGain;
	m_pValueTreeState->SetGainLevel(m_RMS);

	const ParameterSettings params = m_pValueTreeState->GetParameterSettings();

	auto leftGain = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
	auto rightGain = buffer.getRMSLevel(1, 0, buffer.getNumSamples());

	leftGain += params.FaderLevel;
	rightGain += params.FaderLevel;

	m_LeftChannel.get<1>().setGainDecibels(leftGain);
	m_RightChannel.get<1>().setGainDecibels(rightGain);

	m_LeftChannel.get<1>().setRampDurationSeconds(params.Attack / 100);// attack is in ms
	m_RightChannel.get<1>().setRampDurationSeconds(params.Attack / 100);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new Fader_RiderAudioProcessor();
}
