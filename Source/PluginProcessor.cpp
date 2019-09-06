/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterAudioProcessor::FilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
	parameters(*this, nullptr, "PARAMETER", createParameterLayout())
#endif
{
	//parameters.state = ValueTree("savedparams");
}

FilterAudioProcessor::~FilterAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout FilterAudioProcessor::createParameterLayout()
{
	std::vector <std::unique_ptr<RangedAudioParameter>> params;

	auto freqParam = std::make_unique<AudioParameterFloat>(FREQ_ID, FREQ_NAME, 20.0f, 20000.0f, 100.0f);
	auto qParam = std::make_unique<AudioParameterFloat>(Q_ID, Q_NAME, 0.001f, 1.0f, 0.1f);
	auto volumeParam = std::make_unique<AudioParameterFloat>(VOLUME_ID, VOLUME_NAME, -48.0f, 15.0f, 0.0f);
	auto peakGainParam = std::make_unique<AudioParameterFloat>(PEAKGAIN_ID, PEAKGAIN_NAME, -48.0f, 30.0f, 0.0f);
	auto filterChoiceParam = std::make_unique<AudioParameterFloat>(CHOICE_ID, CHOICE_NAME, NormalisableRange<float>(1, 4, 1), 1);
	auto biquadChoiceParam = std::make_unique<AudioParameterFloat>(BIQCHOICE_ID, BIQCHOICE_NAME, NormalisableRange<float>(1, 7, 1), 1);

	params.push_back(std::move(freqParam));
	params.push_back(std::move(qParam));
	params.push_back(std::move(volumeParam));
	params.push_back(std::move(peakGainParam));
	params.push_back(std::move(filterChoiceParam));
	params.push_back(std::move(biquadChoiceParam));

	return { params.begin(), params.end() };
}



//==============================================================================
const String FilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterAudioProcessor::setCurrentProgram (int index)
{
}

const String FilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	for (int channel = 0; channel < kChannels; channel++)
	{
		filter[channel].prepareToPlay(sampleRate, samplesPerBlock);
		biQ[channel].setBiquad(bq_type_lowpass, 0.5, 0.707, 0);
	}
	
}

void FilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif



void FilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	
	//auto sliderFreqValue = parameters.getRawParameterValue(FREQ_ID);
	//auto sliderqValue = parameters.getRawParameterValue(Q_ID);
	//auto sliderVolumeValue = parameters.getRawParameterValue(VOLUME_ID);
	float sliderFreqValue = ((*parameters.getRawParameterValue(FREQ_ID) / 20000) * 0.49);
	float sliderFrequencyValue = *parameters.getRawParameterValue(FREQ_ID);
	float sliderQValue = *parameters.getRawParameterValue(Q_ID);
	float sliderVolumeValue = *parameters.getRawParameterValue(VOLUME_ID);
	float sliderPeakGainValue = *parameters.getRawParameterValue(PEAKGAIN_ID);

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		//input data
		const float* inputData = buffer.getReadPointer(channel);

		float* outputData = buffer.getWritePointer(channel);
		
		//place audio samples into buffer
		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			//get current value from read pointer
			float inputSample = inputData[sample];

			switch ((int)filterChoice) {
			case FIRHiPass:

				outputData[sample] = filter[channel].simpleFIRHiPass(inputSample, sliderFreqValue);
				break;

			case IIRLowPass:

				outputData[sample] = filter[channel].simpleIIRLowPass(inputSample, sliderFreqValue);
				break;

			case BiquadFilter:
				filter[channel].setBiquad((int)biquadChoice, sliderFrequencyValue, sliderQValue, sliderPeakGainValue);
				outputData[sample] = filter[channel].processBiquad(inputSample);
				//filter[channel].setButterworth(sliderFrequencyValue);
				//outputData[channel] = filter[channel].processButterworth(inputSample);
				break;
			case Bilinear:
				filter[channel].setButterworth((int)biquadChoice, sliderFrequencyValue, sliderQValue, sliderPeakGainValue);
				outputData[sample] = filter[channel].processButterworth(inputSample);
			}
			outputData[sample] = outputData[sample] * Decibels::decibelsToGain(sliderVolumeValue);
		}
	}
}

//==============================================================================
bool FilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FilterAudioProcessor::createEditor()
{
    return new FilterAudioProcessorEditor (*this);
}

//==============================================================================
void FilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

	std::unique_ptr<XmlElement> xml(parameters.state.createXml());
	copyXmlToBinary(*xml, destData);
}

void FilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
	std::unique_ptr<XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));

	if (theParams != nullptr)
	{
		if (theParams->hasTagName(parameters.state.getType()))
		{
			parameters.state = ValueTree::fromXml(*theParams);
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterAudioProcessor();
}
