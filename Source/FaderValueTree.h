#pragma once
#include <JuceHeader.h>

struct ParameterSettings
{
	float TargetLevel{ 1.f };
	float FaderLevel{ 1.f };
	float RangeMax{ 1.f };
	float RangeMin{ 0.f };
	float VocalSensitivity{ 0.5f };
	float MusicSensitivity{ 0.2f };
	float Output{ 1.f };
	float Attack{ 0.f };

	void DbToGain();

};

class FaderValueTree : public juce::AudioProcessorValueTreeState
{
public:
	FaderValueTree(juce::AudioProcessor& audioProcessor);

	void UpdateParameterSettings();
	void SetGainLevel(float currentGain);

	ParameterSettings GetParameterSettings() const { return m_Parameters; }
private:
	ParameterSettings m_Parameters{};
	const float m_Smoothing{20.f};

	ParameterLayout CreateParameterLayout();


};
