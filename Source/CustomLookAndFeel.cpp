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

	if (sliderStyle == juce::Slider::TwoValueVertical)
	{
		DrawTwoValueSlider(g, sliderRect, minSliderPos, maxSliderPos, slider);
	}
	else
	{
		g.setColour(slider.findColour((juce::Slider::backgroundColourId)));

		g.fillRoundedRectangle(sliderRect.toFloat(), static_cast<float>(width) / 4.f);
		if (const auto pSlider = dynamic_cast<CustomSlider*>(&slider); pSlider)
		{
			g.setColour(slider.findColour(juce::Slider::thumbColourId));
			const auto thumbHeight = pSlider->GetThumbWidth();

			if (sliderStyle == juce::Slider::LinearVertical)
			{
				const auto thumbRect = juce::Rectangle{ static_cast<float>(x), sliderPos - thumbHeight / 2,
														static_cast<float>(sliderRect.getWidth()),thumbHeight
				};

				g.fillRoundedRectangle(thumbRect, thumbHeight / 4.f);
			}
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

	if (const auto pSlider = dynamic_cast<CustomSlider*>(&slider); pSlider)
	{
		juce::Font font{};
		font.setHeight(pSlider->GetTextHeight());
		g.setFont(font);
		const auto text = pSlider->GetName();
		auto textWidth = g.getCurrentFont().getStringWidth(text);

		g.setColour(juce::Colours::black);
		g.drawFittedText(text, circleBounds.getCentreX() - textWidth / 2, y - slider.getTextBoxHeight(), textWidth,
		                 static_cast<int>(font.getHeight()), juce::Justification::centred, 1);
	}
}

void CustomLookAndFeel::DrawTwoValueSlider(juce::Graphics& g, juce::Rectangle<int> sliderRect, float minSliderPos,
	float maxSliderPos, juce::Slider& slider)
{
	sliderRect.translate(-20.f, 0.f);
	const auto width = static_cast<float>(sliderRect.getWidth());
	const auto x = static_cast<float>(sliderRect.getX());
	g.setColour(slider.findColour((juce::Slider::backgroundColourId)));

	g.fillRoundedRectangle(sliderRect.toFloat(), width / 4.f);

	if (const auto pSlider = dynamic_cast<CustomSlider*>(&slider); pSlider)
	{


		//Range-----------------
		const auto rangeRect = juce::Rectangle{ x, maxSliderPos,
			width, minSliderPos - maxSliderPos
		};

		g.setColour(slider.findColour(juce::Slider::trackColourId));
		g.fillRoundedRectangle(rangeRect, width / 4.f);

		//Thumbs-------------
		g.setColour(slider.findColour(juce::Slider::thumbColourId));

		const auto thumbHeight = pSlider->GetThumbWidth();
		const float diameter = thumbHeight;

		const auto maxThumbRect = juce::Rectangle{
			static_cast<float>(x) - diameter, maxSliderPos - thumbHeight / 2,
			diameter, diameter
		};

		const auto minThumbRect = juce::Rectangle{
			x + width, minSliderPos - thumbHeight / 2,
			diameter, diameter
		};

		drawPointer(g, maxThumbRect.getX(), maxThumbRect.getY(),
			diameter, slider.findColour(juce::Slider::thumbColourId), 1);

		drawPointer(g, minThumbRect.getX(), minThumbRect.getY(),
			diameter, slider.findColour(juce::Slider::thumbColourId), 3);

		//Text-----------------
		juce::Font font{};
		font.setHeight(pSlider->GetTextHeight());
		g.setFont(font);
		const auto minText = pSlider->GetTextStr();
		auto textWidth = g.getCurrentFont().getStringWidth(minText);

		g.drawFittedText(minText, static_cast<int>(minThumbRect.getRight()), static_cast<int>(minThumbRect.getY()),
			textWidth, static_cast<int>(font.getHeight()),
			juce::Justification::centred, 1);

		const auto maxText = pSlider->GetTextStr(false);
		textWidth = g.getCurrentFont().getStringWidth(maxText);

		g.drawFittedText(maxText, static_cast<int>(minThumbRect.getRight()), static_cast<int>(maxThumbRect.getY()),
			textWidth, static_cast<int>(font.getHeight()),
			juce::Justification::centred, 1);

		const auto name = pSlider->GetName();
		textWidth = g.getCurrentFont().getStringWidth(name);

		g.drawFittedText(name, sliderRect.getX(), 2.f, textWidth, static_cast<int>(font.getHeight()),
			juce::Justification::centred, 1);
	}
}


