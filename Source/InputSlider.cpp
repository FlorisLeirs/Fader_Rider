/*
  ==============================================================================

	InputSlider.cpp
	Created: 14 Jun 2023 2:24:10pm
	Author:  floris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputSlider.h"

void InputLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float, float, const juce::Slider::SliderStyle,
	juce::Slider& slider)
{
	auto sliderBounds = juce::Rectangle(x, y, width, height).toFloat();

	if (const auto pSlider = dynamic_cast<InputSlider*>(&slider); pSlider)
	{
		const auto range = pSlider->getRange();

		//Background------------------
		g.setColour(juce::Colours::lightgrey);
		g.fillRect(sliderBounds);

		//Input volume----------------------
		const auto mappedLevel = juce::jmap(static_cast<double>(pSlider->GetInputLevel()), range.getStart(), range.getEnd(),
			static_cast<double>(pSlider->GetSliderOffset()),
			static_cast<double>(sliderBounds.getWidth() + pSlider->GetSliderOffset()));

		const auto center = sliderBounds.getCentre().toInt();

		sliderBounds.removeFromRight(sliderBounds.getWidth() - static_cast<float>(mappedLevel));

		g.setColour(juce::Colours::goldenrod);
		g.fillRect(sliderBounds);

		//Thumb--------------------
		const auto thumbWidth = pSlider->GetThumbWidth();
		const auto thumbRect = juce::Rectangle<float>{
			sliderPos - thumbWidth / 2.f, sliderBounds.getY(), thumbWidth, sliderBounds.getHeight()
		};

		g.setColour(juce::Colours::white);
		g.fillRoundedRectangle(thumbRect, 2.f);

		//Text--------------------------------
		juce::Font font{};
		font.setHeight(pSlider->GetTextHeight());
		font.setBold(true);
		g.setFont(font);
		const auto text = pSlider->GetName();
		const auto textWidth = g.getCurrentFont().getStringWidth(text);

		juce::Rectangle<int> textRect{};
		textRect.setSize(textWidth, pSlider->getTextBoxHeight());
		textRect.setCentre(center);
		textRect.setY(0);
		textRect.translate(-slider.getTextBoxWidth() / 2 - textWidth, 0);

		g.setColour(juce::Colours::black);
		g.drawFittedText(text, textRect, juce::Justification::centred, 1);
	}
}

//==============================================================================
InputSlider::InputSlider()
{
	setScrollWheelEnabled(true);
	setSliderStyle(LinearHorizontal);
	setTextValueSuffix("dB");

	m_pLookAndFeel = std::make_unique<InputLookAndFeel>();
	setLookAndFeel(m_pLookAndFeel.get());
}

InputSlider::~InputSlider()
{
	setLookAndFeel(nullptr);
}

void InputSlider::paint(juce::Graphics& g)
{
	const auto range = getRange();
	const auto sliderBounds = GetSliderBounds().toNearestInt();

	const auto sliderPos = juce::jmap(getValue(), range.getStart(), range.getEnd(),
		static_cast<double>(GetSliderOffset()),
		static_cast<double>(static_cast<float>(sliderBounds.getWidth()) + GetSliderOffset()));

	m_pLookAndFeel->drawLinearSlider(g, sliderBounds.getX(), sliderBounds.getY(), sliderBounds.getWidth(),
		sliderBounds.getHeight(), static_cast<float>(sliderPos), 0, 0,
		getSliderStyle(), *this);
}

void InputSlider::resized()
{
	CustomSlider::resized();
}

juce::Rectangle<float> InputSlider::GetSliderBounds() const
{
	auto bounds = getLocalBounds().toFloat();

	bounds.removeFromTop(static_cast<float>(getTextBoxHeight()));
	bounds.removeFromLeft(GetOffset());
	bounds.removeFromRight(GetOffset());
	bounds.removeFromBottom(GetOffset());

	return bounds;
}
