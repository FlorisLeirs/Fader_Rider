/*
  ==============================================================================

	InputSlider.h
	Created: 14 Jun 2023 2:24:10pm
	Author:  floris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomSlider.h"

class InputLookAndFeel final : public juce::LookAndFeel_V4
{
public:
	InputLookAndFeel() = default;
	~InputLookAndFeel() override = default;
	InputLookAndFeel(InputLookAndFeel& other) = delete;
	InputLookAndFeel(InputLookAndFeel&& other) = delete;
	InputLookAndFeel& operator=(InputLookAndFeel& other) = delete;
	InputLookAndFeel& operator=(InputLookAndFeel&& other) = delete;

	void drawLinearSlider(juce::Graphics&, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		juce::Slider::SliderStyle,
		juce::Slider&) override;
};

//==============================================================================
/*
*/
class InputSlider final : public CustomSlider
{
public:
	InputSlider();
	~InputSlider() override;
	InputSlider(InputSlider& other) = delete;
	InputSlider(InputSlider&& other) = delete;
	InputSlider& operator=(InputSlider& other) = delete;
	InputSlider& operator=(InputSlider&& other) = delete;

	void paint(juce::Graphics&) override;
	void resized() override;
	juce::Rectangle<float> GetSliderBounds() const override;

	void SetInputLevel(float inputLevel) { m_InputLevel = inputLevel; }

	float GetInputLevel() const { return m_InputLevel; }

private:
	std::unique_ptr<InputLookAndFeel> m_pLookAndFeel = nullptr;
	float m_InputLevel{ -15.f };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputSlider)
};
