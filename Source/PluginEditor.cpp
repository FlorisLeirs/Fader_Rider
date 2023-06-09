/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using juce::Slider;
//==============================================================================
Fader_RiderAudioProcessorEditor::Fader_RiderAudioProcessorEditor(Fader_RiderAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 600);

	m_MinMaxSlider.setSliderStyle(Slider::TwoValueVertical);
	m_MinMaxSlider.setMinValue(p.m_pValueTreeState.GetParameterSettings().RangeMin);
	m_MinMaxSlider.setMaxValue(p.m_pValueTreeState.GetParameterSettings().RangeMax);

	m_FaderLevel.setSliderStyle(Slider::LinearVertical);
	m_FaderLevel.setValue(p.m_pValueTreeState.GetParameterSettings().FaderLevel);

	m_OutputSlider.setSliderStyle(Slider::LinearVertical);
	m_OutputSlider.setValue(p.m_pValueTreeState.GetParameterSettings().Output);

	m_TargetLevel.setSliderStyle(Slider::LinearHorizontal);
	m_TargetLevel.setValue(p.m_pValueTreeState.GetParameterSettings().TargetLevel);

	m_VocalSensitivity.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	m_VocalSensitivity.setValue(p.m_pValueTreeState.GetParameterSettings().VocalSensitivity);

	m_AttackKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	m_AttackKnob.setValue(p.m_pValueTreeState.GetParameterSettings().Attack);

	addAndMakeVisible(m_MinMaxSlider);
	addAndMakeVisible(m_FaderLevel);
	addAndMakeVisible(m_OutputSlider);
	addAndMakeVisible(m_TargetLevel);
	addAndMakeVisible(m_VocalSensitivity);
	addAndMakeVisible(m_AttackKnob);
}

Fader_RiderAudioProcessorEditor::~Fader_RiderAudioProcessorEditor()
{
}

//==============================================================================
void Fader_RiderAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::white);
	g.setFont(15.0f);
	//g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);

}

void Fader_RiderAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	auto bounds = getLocalBounds();
	auto topArea = bounds.removeFromTop(bounds.getHeight() / 3);

	m_TargetLevel.setBounds(topArea.removeFromTop(topArea.getHeight() / 2));
	m_AttackKnob.setBounds(topArea.removeFromLeft(topArea.getWidth() / 3));
	m_VocalSensitivity.setBounds(topArea.removeFromLeft(topArea.getWidth() / 2));

	m_MinMaxSlider.setBounds(bounds.removeFromLeft(bounds.getWidth() / 3));
	m_FaderLevel.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
	m_OutputSlider.setBounds(bounds);
}
