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

void InputLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle,
	juce::Slider&)
{
	const auto bounds = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y),
		static_cast<float>(width), static_cast<float>(height));

	g.setColour(juce::Colours::yellowgreen);
	g.fillRect(bounds);

	g.setColour(juce::Colours::lightgrey);
	g.drawRect(bounds, 2.f);
}

//==============================================================================
InputSlider::InputSlider()
	: Slider()
{
	setSliderStyle(Slider::LinearHorizontal);
	setTextValueSuffix("dB");
	setTextBoxStyle(Slider::TextBoxAbove, false, 50, 25);

	m_pLookAndFeel = std::make_unique<InputLookAndFeel>();
	setLookAndFeel(m_pLookAndFeel.get());
}

InputSlider::~InputSlider()
{
	setLookAndFeel(nullptr);
}

void InputSlider::paint(juce::Graphics& g)
{

	auto range = getRange();
	auto sliderBounds = GetSliderBounds();

	//getLookAndFeel().drawLinearSlider(g, sliderBounds.getX(), sliderBounds.getY(),
	//	sliderBounds.getWidth(),
	//	sliderBounds.getHeight(),
	//	juce::jmap(getValue(), range.getStart(), range.getEnd(), 0.0, 1.0),
	//	sliderBounds.getX(), sliderBounds.getRight(),
	//	getSliderStyle(), *this);

	g.setColour(juce::Colours::grey);
	g.fillRect(sliderBounds);

	g.setColour(juce::Colours::lightgrey);
	const auto levelX = juce::jmap(static_cast<double>(m_InputLevel), range.getStart(), range.getEnd(),
		0.0, (double)sliderBounds.getWidth());

	g.fillRect(sliderBounds.removeFromLeft(levelX));
}

void InputSlider::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

}

juce::Rectangle<float> InputSlider::GetSliderBounds() const
{
	return getLocalBounds().toFloat();
}
