/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <fstream>

//==============================================================================
/**
*/
class FilterAudioProcessorEditor  : public AudioProcessorEditor,
									private ComboBox::Listener
{
public:
	FilterAudioProcessorEditor (FilterAudioProcessor&);
    ~FilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void comboBoxChanged(ComboBox*) override;
	
private:
	Slider freqSlider;
	Slider qSlider;
	Slider peakGainSlider;
	Slider volumeSlider;
	ComboBox filterChoice;
	ComboBox biquadChoice;

	std::ofstream bufferFile;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
	FilterAudioProcessor& processor;

public:
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqSliderVal;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qSliderVal;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> volSliderVal;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> peakGainSliderVal;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> filterChoiceVal;
	std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> biquadChoiceVal;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};
