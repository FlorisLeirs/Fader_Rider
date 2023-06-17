/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "FaderValueTree.h"
#include "3rdParty/MultiValueAttachedControlBase.h"
#include "InputSlider.h"

using juce::Slider;
//==============================================================================
Fader_RiderAudioProcessorEditor::Fader_RiderAudioProcessorEditor(Fader_RiderAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
	, m_pMinMaxSlider(std::make_unique<CustomSlider>())
	, m_pOutputSlider(std::make_unique<CustomSlider>())
	, m_pFaderLevel(std::make_unique<CustomSlider>())
	, m_pTargetLevel(std::make_unique<InputSlider>())
	, m_pThresholdSlider(std::make_unique<CustomSlider>())
	, m_pRampSlider(std::make_unique<CustomSlider>())
	, m_OutputAttachment(*p.GetValueTree(), ParameterSettings::OutputStr, *m_pOutputSlider)
	, m_FaderAttachment(*p.GetValueTree(), ParameterSettings::FaderStr, *m_pFaderLevel)
	, m_TargetAttachment(*p.GetValueTree(), ParameterSettings::TargetStr, *m_pTargetLevel)
	, m_ThresholdAttachment(*p.GetValueTree(), ParameterSettings::ThresholdStr, *m_pThresholdSlider)
	, m_RampAttachment(*p.GetValueTree(), ParameterSettings::RampStr, *m_pRampSlider)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 600);
	InitializeSliders();
	m_pMinMaxAttachment = std::make_unique<TwoValueSliderAttachment>(*p.GetValueTree(), ParameterSettings::MinStr,
		ParameterSettings::MaxStr, *m_pMinMaxSlider);
	startTimerHz(24);

	setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::darkslategrey);
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
	m_pRampSlider->setBounds(topArea.removeFromLeft(topArea.getWidth() / 2));
	m_pThresholdSlider->setBounds(topArea.removeFromLeft(topArea.getWidth()));

	m_pMinMaxSlider->setBounds(bounds.removeFromLeft(bounds.getWidth() / 3));
	m_pFaderLevel->setBounds(bounds.removeFromLeft(bounds.getWidth() / 2));
	m_pOutputSlider->setBounds(bounds);

}

void Fader_RiderAudioProcessorEditor::timerCallback()
{
	m_pTargetLevel->SetInputLevel(audioProcessor.GetRMS());
	m_pTargetLevel->repaint();
}

void Fader_RiderAudioProcessorEditor::InitializeSliders()
{
	m_pMinMaxSlider->setSliderStyle(Slider::TwoValueVertical);
	m_pMinMaxSlider->setMinValue(audioProcessor.GetValueTree()->GetParameterSettings().RangeMin);
	m_pMinMaxSlider->setMaxValue(audioProcessor.GetValueTree()->GetParameterSettings().RangeMax);
	m_pMinMaxSlider->setTextValueSuffix("dB");
	m_pMinMaxSlider->setTextBoxStyle(Slider::NoTextBox, false, 50, 25);

	m_pFaderLevel->setSliderStyle(Slider::LinearVertical);
	m_pFaderLevel->setValue(audioProcessor.GetValueTree()->GetParameterSettings().FaderLevel);
	m_pFaderLevel->setTextValueSuffix("dB");
	m_pFaderLevel->setTextBoxStyle(Slider::TextBoxAbove, true, 50, 25);

	m_pOutputSlider->setSliderStyle(Slider::LinearVertical);
	m_pOutputSlider->setValue(audioProcessor.GetValueTree()->GetParameterSettings().Output);
	m_pOutputSlider->setTextValueSuffix("dB");
	m_pOutputSlider->setTextBoxStyle(Slider::TextBoxAbove, false, 50, 25);

	m_pTargetLevel->setValue(audioProcessor.GetValueTree()->GetParameterSettings().TargetLevel);

	m_pThresholdSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	m_pThresholdSlider->setValue(audioProcessor.GetValueTree()->GetParameterSettings().Threshold);
	m_pThresholdSlider->SetValueName("Threshold: ");
	m_pThresholdSlider->setTextValueSuffix("dB");
	m_pThresholdSlider->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 25);

	m_pRampSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	m_pRampSlider->setValue(audioProcessor.GetValueTree()->GetParameterSettings().Ramp);
	m_pRampSlider->SetValueName("Ramp: ");
	m_pRampSlider->setTextValueSuffix("ms");
	m_pRampSlider->setTextBoxStyle(Slider::TextBoxAbove, false, 150, 25);

	addAndMakeVisible(*m_pMinMaxSlider);
	addAndMakeVisible(*m_pFaderLevel);
	addAndMakeVisible(*m_pOutputSlider);
	addAndMakeVisible(*m_pTargetLevel);
	addAndMakeVisible(*m_pThresholdSlider);
	addAndMakeVisible(*m_pRampSlider);
}
