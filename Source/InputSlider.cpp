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
	float /*minSliderPos*/, float /*maxSliderPos*/, const juce::Slider::SliderStyle,
	juce::Slider& slider)
{
	auto sliderBounds = juce::Rectangle(x, y, width, height).toFloat();

	if (auto pSlider = dynamic_cast<InputSlider*>(&slider); pSlider)
	{
		auto range = pSlider->getRange();

		g.setColour(juce::Colours::lightgrey);
		g.fillRect(sliderBounds);

		g.setColour(juce::Colours::goldenrod);
		const auto levelX = juce::jmap(static_cast<double>(pSlider->GetInputLevel()), range.getStart(), range.getEnd(),
		                               static_cast<double>(pSlider->GetSliderOffset()),
		                               static_cast<double>(sliderBounds.getWidth() + pSlider->GetSliderOffset()));

		auto center = sliderBounds.getCentre().toInt();

		sliderBounds.removeFromRight(sliderBounds.getWidth() - static_cast<float>(levelX));


		g.fillRect(sliderBounds);

		g.setColour(juce::Colours::white);
		auto thumbWidth = pSlider->GetThumbWidth();
		auto thumbRect = juce::Rectangle<float>{
			sliderPos - thumbWidth / 2.f, sliderBounds.getY(), thumbWidth, sliderBounds.getHeight()
		};

		g.fillRoundedRectangle(thumbRect, 2.f);

		juce::Font font{};
		font.setHeight(pSlider->GetTextHeight());
		font.setBold(true);
		g.setFont(font);
		const auto text = pSlider->GetTextStr();
		const auto textWidth = g.getCurrentFont().getStringWidth(text);

		juce::Rectangle<int> textRect{};
		textRect.setSize(textWidth, static_cast<int>(pSlider->GetTextHeight()));
		textRect.setCentre(center);
		textRect.translate(0, static_cast<int>(-sliderBounds.getHeight()));

		g.setColour(juce::Colours::black);
		g.drawFittedText(text, textRect, juce::Justification::centredTop, 1);

	}
}

//==============================================================================
InputSlider::InputSlider()
	: CustomSlider()
{
	setScrollWheelEnabled(true);
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

	const auto sliderPos = juce::jmap(getValue(), range.getStart(), range.getEnd(),
		static_cast<double>(GetSliderOffset()), static_cast<double>(sliderBounds.getWidth() + GetSliderOffset()));

	m_pLookAndFeel->drawLinearSlider(g, static_cast<int>(sliderBounds.getX()), static_cast<int>(sliderBounds.getY()),
		static_cast<int>(sliderBounds.getWidth()),
		static_cast<int>(sliderBounds.getHeight()), static_cast<float>(sliderPos), 0, 0,
		getSliderStyle(), *this);


}

void InputSlider::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	

}
