/*
  ==============================================================================

	InputSlider.h
	Created: 14 Jun 2023 2:24:10pm
	Author:  floris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class InputLookAndFeel final : public juce::LookAndFeel_V4
{
public:
	InputLookAndFeel();
	~InputLookAndFeel() override;

	void drawLinearSlider(juce::Graphics&,
		int x, int y, int width, int height,
		float sliderPos,
		float minSliderPos,
		float maxSliderPos,
		const juce::Slider::SliderStyle,
		juce::Slider&) override;

private:

};

//==============================================================================
/*
*/
class InputSlider final : public juce::Slider
{
public:
	InputSlider();
	~InputSlider() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void SetInputLevel(float inputLevel) { m_InputLevel = inputLevel; }
	juce::Rectangle<float> GetSliderBounds() const;

private:
	std::unique_ptr<InputLookAndFeel> m_pLookAndFeel = nullptr;
	float m_InputLevel{-15.f};
	float m_SliderOffset{5.f};
	float m_ThumbWidth{10.f};
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSlider)
};
