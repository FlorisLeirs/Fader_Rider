/*
  ==============================================================================

    InputSlider.h
    Created: 14 Jun 2023 2:24:10pm
    Author:  flori

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class InputSlider  : public juce::Slider
{
public:
    InputSlider();
    ~InputSlider() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputSlider)
};
