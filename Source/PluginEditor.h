/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel;
class CustomSlider;
class InputSlider;
class TwoValueSliderAttachment;
//==============================================================================
/**
*/
class Fader_RiderAudioProcessorEditor : public juce::AudioProcessorEditor, juce::Timer
{
public:
	Fader_RiderAudioProcessorEditor(Fader_RiderAudioProcessor&);
	~Fader_RiderAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;
	void timerCallback() override; // Used to get RMS

private:
	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
	using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

	Fader_RiderAudioProcessor& audioProcessor;
	juce::Rectangle<int> m_TopArea{};

	std::unique_ptr<CustomLookAndFeel> m_pLookAndFeel = nullptr;

	std::unique_ptr<CustomSlider> m_pMinMaxSlider = nullptr;
	std::unique_ptr<CustomSlider> m_pOutputSlider = nullptr;
	std::unique_ptr<CustomSlider> m_pFaderLevel = nullptr;
	std::unique_ptr<InputSlider> m_pTargetLevel = nullptr;
	std::unique_ptr<CustomSlider> m_pThresholdSlider = nullptr;
	std::unique_ptr<CustomSlider> m_pRampSlider = nullptr;
	juce::ToggleButton m_Bypass;

	std::unique_ptr<TwoValueSliderAttachment> m_pMinMaxAttachment = nullptr;
	SliderAttachment m_OutputAttachment;
	SliderAttachment m_FaderAttachment;
	SliderAttachment m_TargetAttachment;
	SliderAttachment m_ThresholdAttachment;
	SliderAttachment m_RampAttachment;
	ButtonAttachment m_BypassAttachment;

	void InitializeSliders();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fader_RiderAudioProcessorEditor)
};
