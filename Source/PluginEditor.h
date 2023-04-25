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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Fader_RiderAudioProcessorEditor)
};
