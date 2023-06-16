#pragma once
#include <JuceHeader.h>

struct ParameterSettings
{
	float TargetLevel{ 1.f };
	float FaderLevel{ 1.f };
	float RangeMax{ 1.f };
	float RangeMin{ 0.f };
	float Threshold{ 0.5f };
	float MusicSensitivity{ 0.2f };
	float Output{ 1.f };
	float Ramp{ 0.f };

	void DbToGain();

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
