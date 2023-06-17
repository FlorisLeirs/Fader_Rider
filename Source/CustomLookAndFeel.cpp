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

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
	float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
	const int size = juce::jmin(width, height);
	const juce::Rectangle<int> circleBounds = juce::Rectangle<int>{ x, y, width, height }.withSizeKeepingCentre(size, size);

	g.fillEllipse(circleBounds.toFloat());

	g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
	g.drawEllipse(circleBounds.toFloat(), 1.f);

	const auto angle = juce::jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

	const auto thumbOffset = juce::jmin(8.0f, size * 0.5f);
	const auto arcRadius = size / 2 - thumbOffset * 2;

	const juce::Point<float> thumbPoint(
		circleBounds.getCentreX() + arcRadius * std::cos(angle - juce::MathConstants<float>::halfPi),
		circleBounds.getCentreY() + arcRadius * std::sin(angle - juce::MathConstants<float>::halfPi));

	g.setColour(slider.findColour(juce::Slider::thumbColourId));

	const auto thumbWidth = thumbOffset * 2.f;

	g.fillEllipse(juce::Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
}


