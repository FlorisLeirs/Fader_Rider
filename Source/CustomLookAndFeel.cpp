/*
  ==============================================================================

	CustomLookAndFeel.cpp
	Created: 16 Jun 2023 10:59:31am
	Author:  floris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

#include "CustomSlider.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel()
{

}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
	const auto sliderRect = juce::Rectangle{ x, y, width, height };
	g.setColour(slider.findColour((juce::Slider::backgroundColourId)));

	g.fillRoundedRectangle(sliderRect.toFloat(), static_cast<float>(width) / 4.f);

	g.setColour(slider.findColour(juce::Slider::thumbColourId));
	if (const auto pSlider = dynamic_cast<CustomSlider*>(&slider); pSlider)
	{
		const auto thumbHeight = pSlider->GetThumbWidth();

		if (sliderStyle == juce::Slider::LinearVertical)
		{
			auto thumbRect = juce::Rectangle{ static_cast<float>(x), sliderPos - thumbHeight / 2,
		static_cast<float>(sliderRect.getWidth()),thumbHeight
			};

			g.fillRoundedRectangle(thumbRect, thumbHeight / 4.f);
		}
		else if (sliderStyle == juce::Slider::TwoValueVertical)
		{
			//Range
			const auto rangeRect = juce::Rectangle{ static_cast<float>(x), maxSliderPos,
				static_cast<float>(width), minSliderPos - maxSliderPos
			};

			g.setColour(slider.findColour (juce::Slider::trackColourId));
			g.fillRoundedRectangle(rangeRect, static_cast<float>(width) / 4.f);

			//Thumbs
			const auto maxThumbRect = juce::Rectangle{
				static_cast<float>(x + sliderRect.getWidth()), maxSliderPos - thumbHeight / 2,
				static_cast<float>(sliderRect.getWidth()), thumbHeight
			};

			const auto minThumbRect = juce::Rectangle{
				static_cast<float>(x + sliderRect.getWidth()), minSliderPos - thumbHeight / 2,
				static_cast<float>(sliderRect.getWidth()), thumbHeight
			};

			const float diameter = thumbHeight;

			drawPointer(g, static_cast<float>(x) - diameter, maxSliderPos - thumbHeight / 2,
				diameter, slider.findColour(juce::Slider::thumbColourId), 1);

			drawPointer(g, static_cast<float>(x + sliderRect.getWidth()), minSliderPos - thumbHeight / 2,
				diameter, slider.findColour(juce::Slider::thumbColourId), 3);

		}
	}
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


