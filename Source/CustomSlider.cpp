/*
  ==============================================================================

	CustomSlider.cpp
	Created: 16 Jun 2023 11:28:28am
	Author:  floris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomSlider.h"

#include "CustomLookAndFeel.h"

//==============================================================================
CustomSlider::CustomSlider()
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	setSliderSnapsToMousePosition(false);
	m_pBasicLookAndFeel = std::make_unique<CustomLookAndFeel>();
	setLookAndFeel(m_pBasicLookAndFeel.get());
	setColour(thumbColourId, juce::Colours::lightgrey);
	setColour(trackColourId, juce::Colours::dimgrey);

}

CustomSlider::~CustomSlider()
{
	setLookAndFeel(nullptr);
}

void CustomSlider::paint(juce::Graphics& g)
{
	const auto sliderStyle = getSliderStyle();
	auto bounds = GetSliderBounds().toNearestInt();

	if (sliderStyle == Rotary || sliderStyle == RotaryHorizontalDrag || sliderStyle == RotaryHorizontalVerticalDrag ||
		sliderStyle == RotaryVerticalDrag)
	{
		const float startAngle = juce::degreesToRadians(180.f + 60.f);
		const float endAngle = juce::degreesToRadians(180.f - 60.f) + juce::MathConstants<float>::twoPi;

		getLookAndFeel().drawRotarySlider(g, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
			static_cast<float>(valueToProportionOfLength(getValue())), startAngle, endAngle, *this);
	}
	else if (sliderStyle == LinearVertical)
	{
		const int amountToRemove = bounds.getWidth() / 3; // used to remove from right AND left
		bounds.removeFromLeft(amountToRemove);
		bounds.removeFromRight(amountToRemove);

		const auto range = getRange();
		const auto sliderPos = juce::jmap(getValue(), range.getStart(), range.getEnd(),
			static_cast<double>(bounds.getBottom()), static_cast<double>(bounds.getY()));

		getLookAndFeel().drawLinearSlider(g, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
			static_cast<float>(sliderPos),
			0.f, 0.f, sliderStyle, *this);
	}
	else if (sliderStyle == TwoValueVertical)
	{
		const int amountToRemove = bounds.getWidth() / 3; // used to remove from right AND left
		bounds.removeFromLeft(amountToRemove);
		bounds.removeFromRight(amountToRemove);

		const auto range = getRange();
		const auto maxSliderPos = juce::jmap(getMaxValue(), range.getStart(), range.getEnd(),
			static_cast<double>(bounds.getBottom()), static_cast<double>(bounds.getY()));
		const auto minSliderPos = juce::jmap(getMinValue(), range.getStart(), range.getEnd(),
			static_cast<double>(bounds.getBottom()), static_cast<double>(bounds.getY()));

		getLookAndFeel().drawLinearSlider(g, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 0.f,
			static_cast<float>(minSliderPos), static_cast<float>(maxSliderPos), sliderStyle, *this);
	}
}

void CustomSlider::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	juce::Slider::resized();
}

juce::Rectangle<float> CustomSlider::GetSliderBounds() const
{
	auto bounds = getLocalBounds().toFloat();
	const auto textPos = getTextBoxPosition();

	bounds.removeFromTop(textPos == TextBoxAbove || textPos == TextBoxBelow ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromLeft(textPos == TextBoxLeft ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromRight(textPos == TextBoxRight ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromBottom(textPos == TextBoxBelow || textPos == TextBoxAbove ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);

	return bounds;
}

juce::String CustomSlider::GetTextStr(bool min)
{
	const auto sliderStyle = getSliderStyle();

	if (sliderStyle != TwoValueVertical && sliderStyle != TwoValueHorizontal && sliderStyle != ThreeValueHorizontal &&
		sliderStyle != ThreeValueVertical)
		return getTextFromValue(getValue());

	if (min)
		return getTextFromValue(getMinValue());

	return getTextFromValue(getMaxValue());

}
