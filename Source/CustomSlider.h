/*
  ==============================================================================

	CustomSlider.h
	Created: 16 Jun 2023 11:28:28am
	Author:  floris

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel;
//==============================================================================
/*
*/
class CustomSlider : public juce::Slider
{
public:
	CustomSlider();
	virtual ~CustomSlider() override;
	CustomSlider(CustomSlider& other) = delete;
	CustomSlider(CustomSlider&& other) = delete;
	CustomSlider& operator=(CustomSlider& other) = delete;
	CustomSlider& operator=(CustomSlider&& other) = delete;

	virtual void paint(juce::Graphics&) override;
	virtual void resized() override;
	juce::String getTextFromValue(double value) override;

	juce::Rectangle<float> GetSliderBounds() const;

	void SetSliderOffset(float offset) { m_SliderOffset = offset; }
	void SetThumbWidth(float width) { m_ThumbWidth = width; }
	void SetTextHeight(float height) { m_TextHeight = height; }
	void SetValueName(juce::String name) { m_ValueName = name; }

	float GetTextHeight() const { return m_TextHeight; }
	juce::String GetTextStr(bool min = true);
	float GetSliderOffset() const { return m_SliderOffset; }
	float GetThumbWidth() const { return m_ThumbWidth; }

private:
	std::unique_ptr<CustomLookAndFeel> m_pBasicLookAndFeel = nullptr;
	juce::String m_ValueName{ "" };
	float m_SliderOffset{ 15.f };
	float m_ThumbWidth{ 10.f };
	float m_TextHeight{ 15.f };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomSlider)
};
