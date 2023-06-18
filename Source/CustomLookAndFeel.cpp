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
void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
	float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
	const auto sliderRect = juce::Rectangle{ x, y, width, height };

	if (sliderStyle == juce::Slider::TwoValueVertical)
		DrawTwoValueSlider(g, sliderRect, minSliderPos, maxSliderPos, slider);
	else
		DrawSingleValueSlider(g, sliderRect, sliderPos, sliderStyle, slider);
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
	float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
	const int size = juce::jmin(width, height);
	const juce::Rectangle<int> circleBounds = juce::Rectangle<int>{ x, y, width, height }.withSizeKeepingCentre(size, size);

	//Outline--------------
	g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
	g.fillEllipse(circleBounds.toFloat());

	//Circle------------
	g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
	g.drawEllipse(circleBounds.toFloat(), 1.f);

	const auto angle = juce::jmap(sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

	const auto thumbOffset = juce::jmin(8.0f, static_cast<float>(size) * 0.5f);
	const auto arcRadius = static_cast<float>(size) / 2 - thumbOffset * 2;

	const juce::Point<float> thumbPoint(
		static_cast<float>(circleBounds.getCentreX()) + arcRadius * std::cos(angle - juce::MathConstants<float>::halfPi),
		static_cast<float>(circleBounds.getCentreY()) + arcRadius * std::sin(angle - juce::MathConstants<float>::halfPi));

	const auto thumbWidth = thumbOffset * 2.f;

	g.setColour(slider.findColour(juce::Slider::thumbColourId));
	g.fillEllipse(juce::Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));

	if (const auto pSlider = dynamic_cast<CustomSlider*>(&slider); pSlider)
	{
		//Text-------------
		juce::Font font{};
		font.setHeight(pSlider->GetTextHeight());
		g.setFont(font);
		const auto text = pSlider->GetName();
		const auto textWidth = g.getCurrentFont().getStringWidth(text);

		g.setColour(juce::Colours::black);
		g.drawFittedText(text, circleBounds.getCentreX() - textWidth / 2, y - slider.getTextBoxHeight(), textWidth,
			static_cast<int>(font.getHeight()), juce::Justification::centred, 1);
	}
}

void CustomLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggleButton, bool, bool)
{
	const auto bounds = toggleButton.getLocalBounds().toFloat();
	auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
	const auto r = bounds.withSizeKeepingCentre(size, size);

	//Circle-------------------
	g.setColour(juce::Colours::dimgrey);
	g.fillEllipse(r);

	//Power icon-----------------
	const auto angle = 30.f;
	size /= 1.5f;

	juce::Path powerButton;
	powerButton.addCentredArc(r.getCentreX(), r.getCentreY(), size / 2, size / 2, 0.f,
		juce::degreesToRadians(angle), juce::degreesToRadians(360.f - angle), true);

	// add line in middle
	powerButton.startNewSubPath(r.getCentreX(), r.getCentreY() - size / 1.9f);
	powerButton.lineTo(r.getCentre());

	const auto pst = juce::PathStrokeType(size / 10, juce::PathStrokeType::curved);
	const auto colour = toggleButton.findColour(toggleButton.getToggleState()
		? juce::ToggleButton::tickDisabledColourId
		: juce::ToggleButton::tickColourId);

	g.setColour(colour);
	g.strokePath(powerButton, pst);

	//Text--------
	juce::Font font{};
	const float fontHeight = (r.getY() - bounds.getY()) * .25f;
	font.setHeight(fontHeight);
	g.setFont(font);

	g.setColour(toggleButton.findColour(juce::ToggleButton::textColourId));
	g.drawFittedText(toggleButton.getButtonText(), static_cast<int>(bounds.getX()), static_cast<int>(bounds.getY()),
		static_cast<int>(bounds.getWidth()), static_cast<int>(r.getY() - bounds.getY()),
		juce::Justification::centred, 1);
}

void CustomLookAndFeel::DrawTwoValueSlider(juce::Graphics& g, juce::Rectangle<int> sliderRect, float minSliderPos,
	float maxSliderPos, juce::Slider& slider)
{
	sliderRect.translate(-20, 0); // add offset to make space for pointers and values
	const auto width = static_cast<float>(sliderRect.getWidth());
	const auto x = static_cast<float>(sliderRect.getX());

	// Background
	g.setColour(slider.findColour(juce::Slider::backgroundColourId));
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
		const auto thumbHeight = pSlider->GetThumbWidth();
		const float diameter = thumbHeight;

		const auto maxThumbRect = juce::Rectangle{
			x - diameter, maxSliderPos - thumbHeight / 2,
			diameter, diameter
		};

		const auto minThumbRect = juce::Rectangle{
			x + width, minSliderPos - thumbHeight / 2,
			diameter, diameter
		};

		g.setColour(slider.findColour(juce::Slider::thumbColourId));
		drawPointer(g, maxThumbRect.getX(), maxThumbRect.getY(),
			diameter, slider.findColour(juce::Slider::thumbColourId), 1); // MAX

		drawPointer(g, minThumbRect.getX(), minThumbRect.getY(),
			diameter, slider.findColour(juce::Slider::thumbColourId), 3); // MIN

		//Text-----------------
		juce::Font font{};
		font.setHeight(pSlider->GetTextHeight());
		g.setFont(font);

		// Min--
		const auto minText = pSlider->GetTextStr();
		auto textWidth = g.getCurrentFont().getStringWidth(minText);

		g.drawFittedText(minText, static_cast<int>(minThumbRect.getRight()), static_cast<int>(minThumbRect.getY()),
			textWidth, static_cast<int>(font.getHeight()),
			juce::Justification::centred, 1);

		// Max--
		const auto maxText = pSlider->GetTextStr(false);
		textWidth = g.getCurrentFont().getStringWidth(maxText);

		g.drawFittedText(maxText, static_cast<int>(minThumbRect.getRight()), static_cast<int>(maxThumbRect.getY()),
			textWidth, static_cast<int>(font.getHeight()),
			juce::Justification::centred, 1);

		// Name--
		const auto name = pSlider->GetName();
		textWidth = g.getCurrentFont().getStringWidth(name);

		g.drawFittedText(name, sliderRect.getX(), 2, textWidth, static_cast<int>(font.getHeight()),
			juce::Justification::centred, 1);
	}
}

void CustomLookAndFeel::DrawSingleValueSlider(juce::Graphics& g, juce::Rectangle<int> sliderRect, float sliderPos,
	const juce::Slider::SliderStyle sliderStyle, juce::Slider& slider)
{
	const auto width = static_cast<float>(sliderRect.getWidth());

	//Background------------------
	g.setColour(slider.findColour((juce::Slider::backgroundColourId)));
	g.fillRoundedRectangle(sliderRect.toFloat(), width / 4.f);

	if (const auto pSlider = dynamic_cast<CustomSlider*>(&slider); pSlider)
	{
		if (sliderStyle == juce::Slider::LinearVertical)
		{
			const auto x = static_cast<float>(sliderRect.getX());

			const auto thumbHeight = pSlider->GetThumbWidth();
			const auto rounding = thumbHeight / 4.f;

			// Draw filler between 0 value and the sliderpos
			if (!juce::approximatelyEqual(sliderPos, 0.f))
			{
				const auto centreY = static_cast<float>(sliderRect.getCentreY());

				juce::Rectangle<float> fillerRect{};
				fillerRect.setLeft(x);
				fillerRect.setWidth(width);
				if (sliderPos < centreY)
				{
					fillerRect.setTop(sliderPos);
					fillerRect.setHeight(centreY - sliderPos);
				}
				else
				{
					fillerRect.setTop(centreY);
					fillerRect.setHeight(sliderPos - centreY);
				}

				g.setColour(slider.findColour(juce::Slider::ColourIds::trackColourId));
				g.fillRoundedRectangle(fillerRect, rounding);
			}

			//Thumb----------------------
			const auto thumbRect = juce::Rectangle{ x, sliderPos - thumbHeight / 2,width,thumbHeight };

			g.setColour(slider.findColour(juce::Slider::thumbColourId));
			g.fillRoundedRectangle(thumbRect, thumbHeight / 4.f);

			//Text--------------------------
			juce::Font font{};
			font.setHeight(pSlider->GetTextHeight());
			g.setFont(font);
			const auto text = pSlider->GetName();
			const auto textWidth = g.getCurrentFont().getStringWidth(text);

			g.setColour(juce::Colours::white);
			g.drawFittedText(text, sliderRect.getCentreX() - textWidth / 2, sliderRect.getBottom() + 2, textWidth,
				static_cast<int>(font.getHeight()), juce::Justification::centred, 1);
		}
	}
}


