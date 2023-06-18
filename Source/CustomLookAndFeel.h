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
	CustomLookAndFeel();
	~CustomLookAndFeel() override;

	void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider&) override;
	void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider&) override;
	void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
	void DrawTwoValueSlider(juce::Graphics&, juce::Rectangle<int> sliderRect, float minSliderPos, float maxSliderPos, juce::Slider&);
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel)
};
