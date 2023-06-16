/*
  ==============================================================================

    CustomSlider.cpp
    Created: 16 Jun 2023 11:28:28am
    Author:  floris

  ==============================================================================
*/

#include <JuceHeader.h>
#include "CustomSlider.h"

//==============================================================================
CustomSlider::CustomSlider()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSliderSnapsToMousePosition(false);
}

CustomSlider::~CustomSlider()
{
}

void CustomSlider::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("CustomSlider", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void CustomSlider::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

juce::Rectangle<float> CustomSlider::GetSliderBounds() const
{
	//return getLocalBounds().toFloat();

	auto bounds = getLocalBounds().toFloat();
    const auto textPos = getTextBoxPosition();

	bounds.removeFromTop(textPos == TextBoxAbove ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromLeft(textPos == TextBoxLeft ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromRight(textPos == TextBoxRight ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);
	bounds.removeFromBottom(textPos == TextBoxBelow ? static_cast<float>(getTextBoxHeight()) : m_SliderOffset);

	return bounds;
}

juce::String CustomSlider::GetTextStr()
{
	return juce::String(m_ValueName + getTextFromValue(getValue()));
}