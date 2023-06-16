/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 16 Jun 2023 10:59:31am
    Author:  floris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel()
{

}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& graphics, int x, int y, int width, int height, float sliderPos,
	float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
	LookAndFeel_V4::drawLinearSlider(graphics, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, sliderStyle,
	                                 slider);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& graphics, int x, int y, int width, int height,
	float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	LookAndFeel_V4::drawRotarySlider(graphics, x, y, width, height, sliderPosProportional, rotaryStartAngle,
	                                 rotaryEndAngle, slider);
}


