/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Fader_RiderAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Fader_RiderAudioProcessorEditor (Fader_RiderAudioProcessor&);
    ~Fader_RiderAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Fader_RiderAudioProcessor& audioProcessor;

    juce::Slider m_MinMaxSlider{};
    juce::Slider m_OutputSlider{};
    juce::Slider m_FaderLevel{};
    juce::Slider m_TargetLevel{};
    juce::Slider m_VocalSensitivity{};
    juce::Slider m_AttackKnob{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fader_RiderAudioProcessorEditor)
};
