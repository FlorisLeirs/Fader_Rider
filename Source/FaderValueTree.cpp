#include "FaderValueTree.h"

FaderValueTree::FaderValueTree(juce::AudioProcessor& audioProcessor)
	: AudioProcessorValueTreeState(audioProcessor, nullptr, "Parameters", CreateParameterLayout())
{

}

void FaderValueTree::UpdateParameterSettings()
{
	m_Parameters.RangeMin = getRawParameterValue(ParameterSettings::MinStr)->load();
	m_Parameters.RangeMax = getRawParameterValue(ParameterSettings::MaxStr)->load();
	m_Parameters.TargetLevel = getRawParameterValue(ParameterSettings::TargetStr)->load();
	m_Parameters.Threshold = getRawParameterValue(ParameterSettings::ThresholdStr)->load();
	m_Parameters.Output = getRawParameterValue(ParameterSettings::OutputStr)->load();
	m_Parameters.NoiseGateBypass = getRawParameterValue(ParameterSettings::NoiseGateByPassStr)->load();

	const float prevRamp = m_Parameters.Ramp;
	m_Parameters.Ramp = getRawParameterValue(ParameterSettings::RampStr)->load();
	if (!juce::approximatelyEqual(prevRamp, m_Parameters.Ramp))
		m_Smoothing.reset(m_SampleRate, m_Parameters.Ramp / 1000);
}

void FaderValueTree::SetGainLevel(float currentGain)
{
	float targetFader = 0.f;
	if (currentGain > m_Parameters.Threshold)
		targetFader = m_Parameters.TargetLevel - currentGain;

	targetFader = std::clamp(targetFader, m_Parameters.RangeMin, m_Parameters.RangeMax);

	m_Smoothing.setTargetValue(targetFader);
	m_Smoothing.skip(m_SamplesPerBlock);

	m_Parameters.FaderLevel = m_Smoothing.getCurrentValue();

	getParameter(ParameterSettings::FaderStr)->setValueNotifyingHost((m_Parameters.FaderLevel + 12.f) / 24.f);
}

juce::AudioProcessorValueTreeState::ParameterLayout FaderValueTree::CreateParameterLayout()
{
	ParameterLayout layout{};

	//Target
	layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterSettings::TargetStr, ParameterSettings::TargetStr,
		juce::NormalisableRange(-60.f, 0.f, 0.02f),
		-20.f));

	//Fader
	layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterSettings::FaderStr, ParameterSettings::FaderStr,
		juce::NormalisableRange(-12.f, 12.f, 0.005f),
		0.f));

	//Max
	layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterSettings::MaxStr, ParameterSettings::MaxStr,
		juce::NormalisableRange(-12.f, 12.f, 0.2f),
		6.f));

	//Min
	layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterSettings::MinStr, ParameterSettings::MinStr,
		juce::NormalisableRange(-12.f, 12.f, 0.2f),
		-6.f));

	//Threshold
	layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterSettings::ThresholdStr, ParameterSettings::ThresholdStr,
		juce::NormalisableRange(juce::Decibels::gainToDecibels(0.f), 0.f, 0.1f, 2.f),
		-100.f));

	//Output
	layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterSettings::OutputStr, ParameterSettings::OutputStr,
		juce::NormalisableRange(-100.f, 100.f, 0.2f, 0.5f, true),
		0.f));

	//Ramp
	layout.add(std::make_unique<juce::AudioParameterFloat>(ParameterSettings::RampStr, ParameterSettings::RampStr,
		juce::NormalisableRange(1.f, 350.f, 1.f, 1.f),
		100.f));

	//Bypass
	layout.add(std::make_unique<juce::AudioParameterBool>(ParameterSettings::NoiseGateByPassStr,
		ParameterSettings::NoiseGateByPassStr, false));

	return layout;
}


