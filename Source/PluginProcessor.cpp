/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

void ParameterSettings::DbToGain()
{
	TargetLevel = juce::Decibels::decibelsToGain(TargetLevel);

	// Make sure values are between -INF and 0 for conversion then add sign
	FaderLevel = FaderLevel < 0.f
		             ? juce::Decibels::decibelsToGain(fabs(FaderLevel)) * -1.f
		             : juce::Decibels::decibelsToGain(fabs(FaderLevel));

	RangeMax = RangeMax < 0.f
		           ? juce::Decibels::decibelsToGain(fabs(RangeMax)) * -1.f
		           : juce::Decibels::decibelsToGain(fabs(RangeMax));

	RangeMin = RangeMin < 0.f
		           ? juce::Decibels::decibelsToGain(fabs(RangeMin)) * -1.f
		           : juce::Decibels::decibelsToGain(fabs(RangeMin));

	VocalSensitivity = juce::Decibels::decibelsToGain(VocalSensitivity);
	MusicSensitivity = juce::Decibels::decibelsToGain(MusicSensitivity);
	Output = juce::Decibels::decibelsToGain(Output);

}

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
	, valueTreeState(*this, nullptr, "Parameters", CreateParameterLayout())
{
	m_Parameters = CreateParameterSettings();
	m_Parameters.DbToGain();
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

void Fader_RiderAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String Fader_RiderAudioProcessor::getProgramName(int index)
{
	return {};
}

void Fader_RiderAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void Fader_RiderAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
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

void Fader_RiderAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

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
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);

		// ..do something to the data...
	}
}

//==============================================================================
bool Fader_RiderAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Fader_RiderAudioProcessor::createEditor()
{
	return new Fader_RiderAudioProcessorEditor(*this);
}

//==============================================================================
void Fader_RiderAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void Fader_RiderAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout Fader_RiderAudioProcessor::CreateParameterLayout()
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout{};

	layout.add(std::make_unique<juce::AudioParameterFloat>("TargetLevel", "TargetLevel",
		juce::NormalisableRange<float>(-30.f, 0.f, 0.2f, 1.f),
		0.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("FaderLevel", "FaderLevel",
		juce::NormalisableRange<float>(-12.f, 12.f, 0.2f, 1.f),
		0.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("RangeMax", "RangeMax",
		juce::NormalisableRange<float>(-12.f, 12.f, 0.2f, 1.f),
		12.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("RangeMin", "RangeMin",
		juce::NormalisableRange<float>(-12.f, 12.f, 0.2f, 1.f),
		-12.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("VocalSensitivity", "VocalSensitivity",
		juce::NormalisableRange<float>(-10.f, -30.f, 0.2f, 1.f),
		-20.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("MusicSensitivity", "MusicSensitivity",
		juce::NormalisableRange<float>(-10.f, -30.f, 0.2f, 1.f),
		-20.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("Output", "Output",
		juce::NormalisableRange<float>(100.f, -100.f, 0.2f, 1.f),
		0.f));
	
	layout.add(std::make_unique<juce::AudioParameterFloat>("Attack", "Attack",
		juce::NormalisableRange<float>(0.f, 500.f, 1.f, 1.f),
		0.f));


	return layout;
}

ParameterSettings Fader_RiderAudioProcessor::CreateParameterSettings()
{
	ParameterSettings params{};

	params.RangeMin = valueTreeState.getRawParameterValue("RangeMin")->load();
	params.RangeMax = valueTreeState.getRawParameterValue("RangeMax")->load();
	params.TargetLevel = valueTreeState.getRawParameterValue("TargetLevel")->load();
	params.FaderLevel = valueTreeState.getRawParameterValue("FaderLevel")->load();
	params.VocalSensitivity = valueTreeState.getRawParameterValue("VocalSensitivity")->load();
	params.MusicSensitivity = valueTreeState.getRawParameterValue("MusicSensitivity")->load();
	params.Output = valueTreeState.getRawParameterValue("Output")->load();
	params.Attack = valueTreeState.getRawParameterValue("Attack")->load();

	return params;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new Fader_RiderAudioProcessor();
}
