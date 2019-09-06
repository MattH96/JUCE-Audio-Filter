#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

enum {
	type_lowpass = 1,
	type_highpass,
	type_bandpass,
	type_notch,
	type_peak,
	type_lowshelf,
	type_highshelf
};

class MyFilter
{
public:
	MyFilter();
	~MyFilter();

	void prepareToPlay(double sampleRate, int samplesPerBlock);

	float simpleFIRHiPass(float currentSample, float sliderValue);
	float simpleIIRHiPass(float currentSample, float sliderValue);
	float simpleIIRLowPass(float currentSample, float sliderValue);

	void setBiquad(int type, double Freq, double Q, double peakGain);
	float processBiquad(float in);

	void setButterworth(int type, double Freq, double Q, double peakGain);
	float processButterworth(float in);
	//Static values, set up in prepare to play.
	int bufferLength;
	int sampleRate;
	double* sn;
	double* cs;

protected:
	void calcBiquad(void);
	void calcButterworth(void);

	//type of filter
	int type;
	//Coefficient values
	float a0, a1, a2, b1, b2 , c0, c1, c2, d1, d2;
	double frequency, Q, peakGain;
	double z1, z2;

	//Post-process delayed samples y[n]
	double delayedSampleY1;
	double delayedSampleY2;
	//Pre-processed delayed samples x[n]
	double delayedSampleX1;
	double delayedSampleX2;
};

inline float MyFilter::processBiquad(float in)
{
	double out = in * a0 + z1;
	z1 = in * a1 + z2 - b1 * out;
	z2 = in * a2 - b2 * out;
	return out;
}

inline float MyFilter::processButterworth(float in)
{
	double V = pow(10, fabs(peakGain) / 20.0);
	double out = in * a0 + delayedSampleX1 * a1 + delayedSampleX2 * a2 - delayedSampleY1 * b1 - delayedSampleY2 * b2;
	delayedSampleX2 = delayedSampleX1;
	delayedSampleY2 = delayedSampleY1;
	delayedSampleX1 = in;
	delayedSampleY1 = out;

	return out;
}