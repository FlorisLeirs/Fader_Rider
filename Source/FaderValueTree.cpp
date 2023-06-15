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
	//m_Parameters.FaderLevel = getRawParameterValue("FaderLevel")->load(); // not loaded because should not be changed in other externally
	m_Parameters.VocalSensitivity = getRawParameterValue("VocalSensitivity")->load();
	m_Parameters.MusicSensitivity = getRawParameterValue("MusicSensitivity")->load();
	m_Parameters.Output = getRawParameterValue("Output")->load();
	m_Parameters.Attack = getRawParameterValue("Attack")->load();

	//m_Parameters.DbToGain();
}

void FaderValueTree::SetGainLevel(float currentGain)
{
	float targetFader = 0.f;
	if (currentGain > m_Parameters.VocalSensitivity)
		targetFader = m_Parameters.TargetLevel - currentGain;

	targetFader = std::clamp(targetFader, m_Parameters.RangeMin, m_Parameters.RangeMax);
	m_Parameters.FaderLevel = std::lerp(m_Parameters.FaderLevel, targetFader,  1.f / m_Smoothing);
	//m_Parameters.FaderLevel = std::lerp(m_Parameters.FaderLevel, targetFader, )
	getParameter("FaderLevel")->setValueNotifyingHost((m_Parameters.FaderLevel + 12.f) / 24.f);
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
		juce::NormalisableRange(-50.f, 0.f, 0.05f, 1.f),
		-25.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("MusicSensitivity", "MusicSensitivity",
		juce::NormalisableRange(-50.f, -10.f, 0.5f, 1.f),
		-25.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("Output", "Output",
		juce::NormalisableRange(-100.f, 100.f, 0.2f, 1.f),
		0.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("Attack", "Attack",
		juce::NormalisableRange(0.f, 500.f, 1.f, 1.f),
		0.f));


	return layout;
}


