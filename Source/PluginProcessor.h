/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MyFilter.h"
#include "Biquad.h"
#define FREQ_ID "freq"
#define FREQ_NAME "Freq"
#define Q_ID "q"
#define Q_NAME "Q"
#define VOLUME_ID "volume"
#define VOLUME_NAME "Volume"
#define CHOICE_ID "choice"
#define CHOICE_NAME "Choice"
#define BIQCHOICE_ID "biqchoice"
#define BIQCHOICE_NAME "BiqChoice"
#define PEAKGAIN_ID "peakGain"
#define PEAKGAIN_NAME "PeakGain"

//==============================================================================
/**
*/

enum
{
	FIRHiPass = 1,
	IIRLowPass,
	BiquadFilter,
	Bilinear
};
class FilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
	FilterAudioProcessor();
    ~FilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	AudioProcessorValueTreeState parameters;
	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	AudioPlayHead* playHead;
	bool playing;
	
	int filterChoice;
	int biquadChoice;

private:
	static const int kChannels = 2;
	MyFilter filter[kChannels];
	Biquad biQ[kChannels];
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessor)
};
