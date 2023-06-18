#pragma once
#include <JuceHeader.h>

struct ParameterSettings
{
	float TargetLevel{ 1.f };
	float FaderLevel{ 1.f };
	float RangeMax{ 1.f };
	float RangeMin{ 0.f };
	float Threshold{ 0.5f };
	float Output{ 1.f };
	float Ramp{ 0.f };
	bool NoiseGateBypass{ false };

	inline static const juce::String TargetStr{"TargetLevel"};
	inline static const juce::String FaderStr{"FaderLevel"};
	inline static const juce::String MaxStr{"RangeMax"};
	inline static const juce::String MinStr{"RangeMin"};
	inline static const juce::String ThresholdStr{"Threshold"};
	inline static const juce::String OutputStr{"Output"};
	inline static const juce::String RampStr{"Ramp"};
	inline static const juce::String NoiseGateByPassStr{"NoiseGateBypass"};
};

class FaderValueTree : public juce::AudioProcessorValueTreeState
{
public:
	FaderValueTree(juce::AudioProcessor& audioProcessor);

	void UpdateParameterSettings();
	void SetGainLevel(float currentGain);
	void SetSampleRate(int sampleRate) { m_SampleRate = sampleRate; }
	void SetSamplesPerBlock(int samplesPerBlock) { m_SamplesPerBlock = samplesPerBlock; }

	ParameterSettings GetParameterSettings() const { return m_Parameters; }
private:
	ParameterSettings m_Parameters{};
	juce::SmoothedValue<float> m_Smoothing{0.f};
	int m_SampleRate{};
	int m_SamplesPerBlock{};

	ParameterLayout CreateParameterLayout();


};
