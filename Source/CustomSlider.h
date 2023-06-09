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
	CustomSlider(bool readOnly = false);
	virtual ~CustomSlider() override;
	CustomSlider(CustomSlider& other) = delete;
	CustomSlider(CustomSlider&& other) = delete;
	CustomSlider& operator=(CustomSlider& other) = delete;
	CustomSlider& operator=(CustomSlider&& other) = delete;

	virtual void paint(juce::Graphics&) override;
	virtual void resized() override;

	virtual juce::Rectangle<float> GetSliderBounds() const;

	void SetSliderOffset(float offset) { m_SliderOffset = offset; }
	void SetThumbWidth(float width) { m_ThumbWidth = width; }
	void SetTextHeight(float height) { m_TextHeight = height; }
	void SetValueName(juce::String name) { m_ValueName = name; }

	float GetTextHeight() const { return m_TextHeight; }
	juce::String GetTextStr(bool min = true);
	const juce::String& GetName() const { return m_ValueName; }
	float GetSliderOffset() const { return m_SliderOffset; }
	float GetThumbWidth() const { return m_ThumbWidth; } // used as height for vertical sliders
	float GetOffset() const { return m_SliderOffset; }

protected:
	void mouseDrag(const juce::MouseEvent& event) override;
	
private:
	juce::String m_ValueName{ "" };
	float m_SliderOffset{ 15.f };
	float m_ThumbWidth{ 10.f };
	float m_TextHeight{ 15.f };
	bool m_ReadOnly;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomSlider)
};
