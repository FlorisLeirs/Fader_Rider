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
	setColour(thumbColourId, juce::Colours::darkslategrey);

}

CustomSlider::~CustomSlider()
{
	setLookAndFeel(nullptr);
}

void CustomSlider::paint(juce::Graphics& g)
{
	const float startAngle = juce::degreesToRadians(180.f + 60.f);
	const float endAngle = juce::degreesToRadians(180.f - 60.f) + juce::MathConstants<float>::twoPi;

	const auto bounds = GetSliderBounds().toNearestInt();
	getLookAndFeel().drawRotarySlider(g, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
		static_cast<float>(getValue()), startAngle, endAngle, *this);
	if(auto label = dynamic_cast<juce::Label*>(getChildComponent(0)); label)
	{
		if(!label->isBeingEdited())
			label->setText(GetTextStr(), juce::dontSendNotification);
		//label.pain
	}
	//getChildComponent(0)->paint(g);
}

void CustomSlider::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	juce::Slider::resized();
}

juce::String CustomSlider::getTextFromValue(double value)
{
	return m_ValueName + Slider::getTextFromValue(value);
}

juce::Rectangle<float> CustomSlider::GetSliderBounds() const
{
	auto bounds = getLocalBounds().toFloat();
	const auto textPos = getTextBoxPosition();

	bounds.removeFromTop(textPos == TextBoxAbove ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromLeft(textPos == TextBoxLeft ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromRight(textPos == TextBoxRight ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromBottom(textPos == TextBoxBelow ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);

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
