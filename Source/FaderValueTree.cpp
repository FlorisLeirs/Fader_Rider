#include "FaderValueTree.h"

void ParameterSettings::DbToGain()
{
	//TargetLevel = TargetLevel < 0.f
	//	             ? juce::Decibels::decibelsToGain(fabs(TargetLevel)) * -1.f
	//	             : juce::Decibels::decibelsToGain(fabs(TargetLevel));
	// Make sure values are between -INF and 0 for conversion then add sign
	//FaderLevel = FaderLevel < 0.f
	//	             ? juce::Decibels::decibelsToGain(fabs(FaderLevel)) * -1.f
	//	             : juce::Decibels::decibelsToGain(fabs(FaderLevel));

	//RangeMax = RangeMax < 0.f
	//	           ? juce::Decibels::decibelsToGain(fabs(RangeMax)) * -1.f
	//	           : juce::Decibels::decibelsToGain(fabs(RangeMax));

	//RangeMin = RangeMin < 0.f
	//	           ? juce::Decibels::decibelsToGain(fabs(RangeMin)) * -1.f
	//	           : juce::Decibels::decibelsToGain(fabs(RangeMin));

	VocalSensitivity = juce::Decibels::decibelsToGain(VocalSensitivity);
	MusicSensitivity = juce::Decibels::decibelsToGain(MusicSensitivity);
	Output = juce::Decibels::decibelsToGain(Output);

}

FaderValueTree::FaderValueTree(juce::AudioProcessor& audioProcessor)
	: AudioProcessorValueTreeState(audioProcessor, nullptr, "Parameters", CreateParameterLayout())
{

}

void FaderValueTree::UpdateParameterSettings()
{
	m_Parameters.RangeMin = getRawParameterValue("RangeMin")->load();
	m_Parameters.RangeMax = getRawParameterValue("RangeMax")->load();
	m_Parameters.TargetLevel = getRawParameterValue("TargetLevel")->load();
	m_Parameters.FaderLevel = getRawParameterValue("FaderLevel")->load();
	m_Parameters.VocalSensitivity = getRawParameterValue("VocalSensitivity")->load();
	m_Parameters.MusicSensitivity = getRawParameterValue("MusicSensitivity")->load();
	m_Parameters.Output = getRawParameterValue("Output")->load();
	m_Parameters.Attack = getRawParameterValue("Attack")->load();

	//m_Parameters.DbToGain();
}

void FaderValueTree::SetGainLevel(float currentGain)
{
	if(currentGain <= -100.f)
	{
		m_Parameters.FaderLevel = 0.f;
		return;
	}
	float gainOffset = m_Parameters.TargetLevel - currentGain;
	gainOffset = std::clamp(gainOffset, m_Parameters.RangeMin, m_Parameters.RangeMax);
	m_Parameters.FaderLevel = gainOffset;
}

juce::AudioProcessorValueTreeState::ParameterLayout FaderValueTree::CreateParameterLayout()
{
	ParameterLayout layout{};

	layout.add(std::make_unique<juce::AudioParameterFloat>("TargetLevel", "TargetLevel",
		juce::NormalisableRange(-30.f, 0.f, 0.2f, 1.f),
		0.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("FaderLevel", "FaderLevel",
		juce::NormalisableRange(-12.f, 12.f, 0.2f, 1.f),
		0.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("RangeMax", "RangeMax",
		juce::NormalisableRange(-12.f, 12.f, 0.2f, 1.f),
		12.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("RangeMin", "RangeMin",
		juce::NormalisableRange(-12.f, 12.f, 0.2f, 1.f),
		-12.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("VocalSensitivity", "VocalSensitivity",
		juce::NormalisableRange(-310.f, -10.f, 0.2f, 1.f),
		-20.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("MusicSensitivity", "MusicSensitivity",
		juce::NormalisableRange(-30.f, -10.f, 0.2f, 1.f),
		-20.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("Output", "Output",
		juce::NormalisableRange(-100.f, 100.f, 0.2f, 1.f),
		0.f));
	
	layout.add(std::make_unique<juce::AudioParameterFloat>("Attack", "Attack",
		juce::NormalisableRange(0.f, 500.f, 1.f, 1.f),
		0.f));


	return layout;
}


