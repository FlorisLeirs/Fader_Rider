/*
  ==============================================================================

	CustomLookAndFeel.h
	Created: 16 Jun 2023 10:59:31am
	Author:  floris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomLookAndFeel final : public juce::LookAndFeel_V4
{
public:
	CustomLookAndFeel() = default;
	~CustomLookAndFeel() override = default;
	CustomLookAndFeel(CustomLookAndFeel& other) = delete;
	CustomLookAndFeel(CustomLookAndFeel&& other) = delete;
	CustomLookAndFeel& operator=(CustomLookAndFeel& other) = delete;
	CustomLookAndFeel& operator=(CustomLookAndFeel&& other) = delete;

	void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos,
		float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider&) override;

	void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height, float sliderPosProportional,
		float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;

	void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted,
		bool shouldDrawButtonAsDown) override;

private:
	void DrawTwoValueSlider(juce::Graphics&, juce::Rectangle<int> sliderRect, float minSliderPos, float maxSliderPos, juce::Slider&);
	void DrawSingleValueSlider(juce::Graphics& g, juce::Rectangle<int> sliderRect, float sliderPos,
		const juce::Slider::SliderStyle sliderStyle, juce::Slider& slider);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
