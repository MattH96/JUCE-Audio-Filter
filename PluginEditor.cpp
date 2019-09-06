/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
#include <fstream>



//==============================================================================
FilterAudioProcessorEditor::FilterAudioProcessorEditor (FilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	freqSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, FREQ_ID, freqSlider);

	freqSlider.setSliderStyle(Slider::LinearHorizontal);
	freqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	freqSlider.setRange(20.0f, 20000.0f);
	addAndMakeVisible(&freqSlider);

	qSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, Q_ID, qSlider);

	qSlider.setSliderStyle(Slider::LinearHorizontal);
	qSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	qSlider.setRange(0.001f, 1.0f);
	addAndMakeVisible(&qSlider);

	volSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, VOLUME_ID, volumeSlider);

	volumeSlider.setSliderStyle(Slider::LinearHorizontal);
	volumeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	volumeSlider.setRange(-48.0f, 15.0f);
	addAndMakeVisible(&volumeSlider);

	peakGainSliderVal = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, PEAKGAIN_ID, peakGainSlider);

	peakGainSlider.setSliderStyle(Slider::LinearHorizontal);
	peakGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	peakGainSlider.setRange(-48.0f, 30.0f);
	addAndMakeVisible(&peakGainSlider);

	filterChoice.addItem("FIRHiPass", 1);
	filterChoice.addItem("IIRLowPass", 2);
	filterChoice.addItem("Biquad", 3);
	filterChoice.addItem("Bilinear", 4);
	filterChoice.setSelectedId(1);
	filterChoice.addListener(this);
	addAndMakeVisible(&filterChoice);

	filterChoiceVal = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters, CHOICE_ID, filterChoice);

	biquadChoice.addItem("Low Pass", 1);
	biquadChoice.addItem("High Pass", 2);
	biquadChoice.addItem("Band Pass", 3);
	biquadChoice.addItem("Notch", 4);
	biquadChoice.addItem("Peak", 5);
	biquadChoice.addItem("Low Shelf", 6);
	biquadChoice.addItem("High Shelf", 7);
	biquadChoice.setSelectedId(1);
	biquadChoice.addListener(this);
	addAndMakeVisible(&biquadChoice);

	biquadChoiceVal = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters, BIQCHOICE_ID, biquadChoice);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500,450);
}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor()
{
}

void FilterAudioProcessorEditor::comboBoxChanged(ComboBox* box)
{
	processor.filterChoice = filterChoice.getSelectedIdAsValue().getValue();
	processor.biquadChoice = biquadChoice.getSelectedIdAsValue().getValue();
}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
	g.drawText("Frequency", (getWidth() / 2) - 200, (((getHeight() / 4) * 1) - 30), 200, 50, Justification::centred, false);
	g.drawText("Q", (getWidth() / 2) - 200, (((getHeight() / 4) * 2) - 80), 200, 50, Justification::centred, false);
	g.drawText("Volume", (getWidth() / 2) - 200, (((getHeight() / 4) * 3) - 130), 200, 50, Justification::centred, false);
	g.drawText("Peak Gain", (getWidth() / 2) - 200, (((getHeight() / 4) * 4) - 180), 200, 50, Justification::centred, false);
	g.drawText("Filter Type:", 120, 5, 80, 50, Justification::centred, false);
	g.drawText("Biquad Type:", 110, 40, 80, 50, Justification::centred, false);
}

void FilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	filterChoice.setBounds(200, 15, 250, 30);
	biquadChoice.setBounds(200, 50, 250, 30);
	freqSlider.setBounds((getWidth() / 2) - 60, (((getHeight() / 4) * 1) - 20), 200, 50);
	qSlider.setBounds((getWidth() / 2) - 60, (((getHeight() / 4) * 2) - 70), 200, 50);
	volumeSlider.setBounds((getWidth() / 2) - 60, (((getHeight() / 4) * 3)- 120), 200, 50);
	peakGainSlider.setBounds((getWidth() / 2) - 60, (((getHeight() / 4) * 4) - 170), 200, 50);
}
