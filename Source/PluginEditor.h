/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

	void timerCallback() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	Fader_RiderAudioProcessor& audioProcessor;

	juce::Rectangle<int> m_TopArea{};

	juce::Slider m_MinMaxSlider{};
	juce::Slider m_OutputSlider{};
	juce::Slider m_FaderLevel{};
	std::unique_ptr<InputSlider> m_pTargetLevel = nullptr;
	std::unique_ptr<CustomSlider> m_pVocalSensitivity = nullptr;
	std::unique_ptr<CustomSlider> m_pAttackKnob = nullptr;

	using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
	std::unique_ptr<TwoValueSliderAttachment> m_pMinMaxAttachment = nullptr;
	Attachment m_OutputAttachment;
	Attachment m_FaderAttachment;
	Attachment m_TargetAttachment;
	Attachment m_VocalAttachment;
	Attachment m_AttackAttachment;


	void InitializeSliders();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Fader_RiderAudioProcessorEditor)
};
