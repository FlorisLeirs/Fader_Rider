/*
  ==============================================================================

	InputSlider.cpp
	Created: 14 Jun 2023 2:24:10pm
	Author:  flori

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputSlider.h"

InputLookAndFeel::InputLookAndFeel()
	:LookAndFeel_V4()
{
}

InputLookAndFeel::~InputLookAndFeel()
{
}

void InputLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float /*sliderPos*/,
	float /*minSliderPos*/, float /*maxSliderPos*/, const juce::Slider::SliderStyle,
	juce::Slider&)
{
	const auto bounds = juce::Rectangle(x, y, width, height).toFloat();

	g.setColour(juce::Colours::yellowgreen);
	g.fillRect(bounds);

	g.setColour(juce::Colours::lightgrey);
	g.drawRect(bounds, 2.f);
}

//==============================================================================
InputSlider::InputSlider()
	: Slider()
{
	setScrollWheelEnabled(true);
	setSliderStyle(Slider::LinearHorizontal);
	setTextValueSuffix("dB");
	setTextBoxStyle(Slider::TextBoxAbove, false, 50, 25);

	m_pLookAndFeel = std::make_unique<InputLookAndFeel>();
	setLookAndFeel(m_pLookAndFeel.get());
	setSliderSnapsToMousePosition(false);
	//setVelocityBasedMode(true);
	//setvel
}

InputSlider::~InputSlider()
{
	setLookAndFeel(nullptr);
}

void InputSlider::paint(juce::Graphics& g)
{

	auto range = getRange();
	auto sliderBounds = GetSliderBounds();

	g.setColour(juce::Colours::grey);
	g.fillRect(sliderBounds);

	g.setColour(juce::Colours::goldenrod);
	const auto levelX = juce::jmap(static_cast<double>(m_InputLevel), range.getStart(), range.getEnd(),
		static_cast<double>(m_SliderOffset), static_cast<double>(sliderBounds.getWidth()));

	const auto sliderPos = juce::jmap(getValue(), range.getStart(), range.getEnd(),
		static_cast<double>(m_SliderOffset), static_cast<double>(sliderBounds.getWidth()));

	g.fillRect(sliderBounds.removeFromLeft(static_cast<float>(levelX)));

	g.setColour(juce::Colours::white);
	auto thumbRect = juce::Rectangle<float>{
		static_cast<float>(sliderPos) - m_ThumbWidth / 2.f, sliderBounds.getY(), m_ThumbWidth, sliderBounds.getHeight()
	};
	g.fillRoundedRectangle(thumbRect, 2.f);
}

void InputSlider::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

}

juce::Rectangle<float> InputSlider::GetSliderBounds() const
{
	//return getLocalBounds().toFloat();

	auto bounds = getLocalBounds().toFloat();
	bounds.removeFromTop(static_cast<float>(getTextBoxHeight()));
	bounds.removeFromLeft(m_SliderOffset);
	bounds.removeFromRight(m_SliderOffset);
	bounds.removeFromBottom(m_SliderOffset);
	return bounds;
}