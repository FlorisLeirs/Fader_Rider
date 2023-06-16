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

	Threshold = juce::Decibels::decibelsToGain(Threshold);
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
	m_Parameters.Threshold = getRawParameterValue("Threshold")->load();
	m_Parameters.MusicSensitivity = getRawParameterValue("MusicSensitivity")->load();
	m_Parameters.Output = getRawParameterValue("Output")->load();

	float prevRamp = m_Parameters.Ramp;
	m_Parameters.Ramp = getRawParameterValue("Ramp")->load();
	if (!juce::approximatelyEqual(prevRamp, m_Parameters.Ramp))
		m_Smoothing.reset(m_SampleRate, m_Parameters.Ramp / 1000);

	//m_Parameters.DbToGain();
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

	getParameter("FaderLevel")->setValueNotifyingHost((m_Parameters.FaderLevel + 12.f) / 24.f);
}

juce::AudioProcessorValueTreeState::ParameterLayout FaderValueTree::CreateParameterLayout()
{
	ParameterLayout layout{};

	layout.add(std::make_unique<juce::AudioParameterFloat>("TargetLevel", "TargetLevel",
		juce::NormalisableRange(-60.f, 0.f, 0.02f, 1.f),
		-20.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("FaderLevel", "FaderLevel",
		juce::NormalisableRange(-12.f, 12.f, 0.005f, 1.f),
		0.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("RangeMax", "RangeMax",
		juce::NormalisableRange(-12.f, 12.f, 0.2f, 1.f),
		12.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("RangeMin", "RangeMin",
		juce::NormalisableRange(-12.f, 12.f, 0.2f, 1.f),
		-12.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("Threshold", "Threshold",
		juce::NormalisableRange(-50.f, 0.f, 0.05f, 1.f),
		-25.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("MusicSensitivity", "MusicSensitivity",
		juce::NormalisableRange(-50.f, -10.f, 0.5f, 1.f),
		-25.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("Output", "Output",
		juce::NormalisableRange(-100.f, 100.f, 0.2f, 1.f),
		0.f));

	layout.add(std::make_unique<juce::AudioParameterFloat>("Ramp", "Ramp",
		juce::NormalisableRange(1.f, 350.f, 1.f, 1.f),
		0.f));


	return layout;
}


