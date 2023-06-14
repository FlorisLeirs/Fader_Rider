/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "3rdParty/MultiValueAttachedControlBase.h"

using juce::Slider;
//==============================================================================
Fader_RiderAudioProcessorEditor::Fader_RiderAudioProcessorEditor(Fader_RiderAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
	, m_OutputAttachment(p.m_pValueTreeState, "Output", m_OutputSlider)
	, m_FaderAttachment(p.m_pValueTreeState, "FaderLevel", m_FaderLevel)
	, m_TargetAttachment(p.m_pValueTreeState, "TargetLevel", m_TargetLevel)
	, m_VocalAttachment(p.m_pValueTreeState, "VocalSensitivity", m_VocalSensitivity)
	, m_AttackAttachment(p.m_pValueTreeState, "Attack", m_AttackKnob)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 600);
	InitializeSliders();
	m_pMinMaxAttachment = std::make_unique<TwoValueSliderAttachment>(p.m_pValueTreeState, "RangeMin", "RangeMax", m_MinMaxSlider );

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

	m_TargetLevel.setBounds(topArea.removeFromTop(topArea.getHeight() / 3));
	m_AttackKnob.setBounds(topArea.removeFromLeft(topArea.getWidth() / 2));
	m_VocalSensitivity.setBounds(topArea.removeFromLeft(topArea.getWidth()));

	m_MinMaxSlider.setBounds(bounds.removeFromLeft(bounds.getWidth() / 3));
	m_FaderLevel.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
	m_OutputSlider.setBounds(bounds);

}

void Fader_RiderAudioProcessorEditor::InitializeSliders()
{
	m_MinMaxSlider.setSliderStyle(Slider::TwoValueVertical);
	m_MinMaxSlider.setMinValue(audioProcessor.m_pValueTreeState.GetParameterSettings().RangeMin);
	m_MinMaxSlider.setMaxValue(audioProcessor.m_pValueTreeState.GetParameterSettings().RangeMax);
	m_MinMaxSlider.setTextValueSuffix("dB");
	m_MinMaxSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 25);

	m_FaderLevel.setSliderStyle(Slider::LinearVertical);
	m_FaderLevel.setValue(audioProcessor.m_pValueTreeState.GetParameterSettings().FaderLevel);
	m_FaderLevel.setTextValueSuffix("dB");
	m_FaderLevel.setTextBoxStyle(Slider::TextBoxAbove, true, 50, 25);

	m_OutputSlider.setSliderStyle(Slider::LinearVertical);
	m_OutputSlider.setValue(audioProcessor.m_pValueTreeState.GetParameterSettings().Output);
	m_OutputSlider.setTextValueSuffix("dB");
	m_OutputSlider.setTextBoxStyle(Slider::TextBoxRight, false, 50, 25);

	m_TargetLevel.setSliderStyle(Slider::LinearHorizontal);
	m_TargetLevel.setValue(audioProcessor.m_pValueTreeState.GetParameterSettings().TargetLevel);
	m_TargetLevel.setTextValueSuffix("dB");
	m_TargetLevel.setTextBoxStyle(Slider::TextBoxAbove, false, 50, 25);

	m_VocalSensitivity.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	m_VocalSensitivity.setValue(audioProcessor.m_pValueTreeState.GetParameterSettings().VocalSensitivity);
	m_VocalSensitivity.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);

	m_AttackKnob.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	m_AttackKnob.setValue(audioProcessor.m_pValueTreeState.GetParameterSettings().Attack);
	m_AttackKnob.setTextValueSuffix("ms");
	m_AttackKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 25);

	addAndMakeVisible(m_MinMaxSlider);
	addAndMakeVisible(m_FaderLevel);
	addAndMakeVisible(m_OutputSlider);
	addAndMakeVisible(m_TargetLevel);
	addAndMakeVisible(m_VocalSensitivity);
	addAndMakeVisible(m_AttackKnob);
}
