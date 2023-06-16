/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "3rdParty/MultiValueAttachedControlBase.h"
#include "InputSlider.h"

using juce::Slider;
//==============================================================================
Fader_RiderAudioProcessorEditor::Fader_RiderAudioProcessorEditor(Fader_RiderAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
	, m_pTargetLevel(std::make_unique<InputSlider>())
	, m_pVocalSensitivity(std::make_unique<CustomSlider>())
	, m_pAttackKnob(std::make_unique<CustomSlider>())
	, m_OutputAttachment(*p.GetValueTree(), "Output", m_OutputSlider)
	, m_FaderAttachment(*p.GetValueTree(), "FaderLevel", m_FaderLevel)
	, m_TargetAttachment(*p.GetValueTree(), "TargetLevel", *m_pTargetLevel)
	, m_VocalAttachment(*p.GetValueTree(), "VocalSensitivity", *m_pVocalSensitivity)
	, m_AttackAttachment(*p.GetValueTree(), "Attack", *m_pAttackKnob)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 600);
	InitializeSliders();
	m_pMinMaxAttachment = std::make_unique<TwoValueSliderAttachment>(*p.GetValueTree(), "RangeMin", "RangeMax", m_MinMaxSlider);
	startTimerHz(24);
}

Fader_RiderAudioProcessorEditor::~Fader_RiderAudioProcessorEditor()
{

}

//==============================================================================
void Fader_RiderAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::grey);
	g.fillRect(m_TopArea);

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
	m_TopArea = topArea;

	m_pTargetLevel->setBounds(topArea.removeFromTop(topArea.getHeight() / 3));
	m_pAttackKnob->setBounds(topArea.removeFromLeft(topArea.getWidth() / 2));
	m_pVocalSensitivity->setBounds(topArea.removeFromLeft(topArea.getWidth()));

	m_MinMaxSlider.setBounds(bounds.removeFromLeft(bounds.getWidth() / 3));
	m_FaderLevel.setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
	m_OutputSlider.setBounds(bounds);

}

void Fader_RiderAudioProcessorEditor::timerCallback()
{
	m_pTargetLevel->SetInputLevel(audioProcessor.GetRMS());
	m_pTargetLevel->repaint();
}

void Fader_RiderAudioProcessorEditor::InitializeSliders()
{
	m_MinMaxSlider.setSliderStyle(Slider::TwoValueVertical);
	m_MinMaxSlider.setMinValue(audioProcessor.GetValueTree()->GetParameterSettings().RangeMin);
	m_MinMaxSlider.setMaxValue(audioProcessor.GetValueTree()->GetParameterSettings().RangeMax);
	m_MinMaxSlider.setTextValueSuffix("dB");
	m_MinMaxSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 50, 25);

	m_FaderLevel.setSliderStyle(Slider::LinearVertical);
	m_FaderLevel.setValue(audioProcessor.GetValueTree()->GetParameterSettings().FaderLevel);
	m_FaderLevel.setTextValueSuffix("dB");
	m_FaderLevel.setTextBoxStyle(Slider::TextBoxAbove, true, 50, 25);

	m_OutputSlider.setSliderStyle(Slider::LinearVertical);
	m_OutputSlider.setValue(audioProcessor.GetValueTree()->GetParameterSettings().Output);
	m_OutputSlider.setTextValueSuffix("dB");
	m_OutputSlider.setTextBoxStyle(Slider::TextBoxRight, false, 150, 25);

	m_pTargetLevel->setValue(audioProcessor.GetValueTree()->GetParameterSettings().TargetLevel);

	m_pVocalSensitivity->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	m_pVocalSensitivity->setValue(audioProcessor.GetValueTree()->GetParameterSettings().VocalSensitivity);
	m_pVocalSensitivity->SetValueName("Threshold: ");
	m_pVocalSensitivity->setTextValueSuffix("dB");
	m_pVocalSensitivity->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 25);

	m_pAttackKnob->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	m_pAttackKnob->setValue(audioProcessor.GetValueTree()->GetParameterSettings().Attack);
	m_pAttackKnob->SetValueName("Ramp: ");
	m_pAttackKnob->setTextValueSuffix("ms");
	m_pAttackKnob->setTextBoxStyle(Slider::TextBoxAbove, false, 50, 25);

	addAndMakeVisible(m_MinMaxSlider);
	addAndMakeVisible(m_FaderLevel);
	addAndMakeVisible(m_OutputSlider);
	addAndMakeVisible(*m_pTargetLevel);
	addAndMakeVisible(*m_pVocalSensitivity);
	addAndMakeVisible(*m_pAttackKnob);
}
