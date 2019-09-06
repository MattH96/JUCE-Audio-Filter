#include "PluginProcessor.h"
#include "MyFilter.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "FastMath.h"
#include "vector"


MyFilter::MyFilter()
{
	sampleRate = 0;
	type = type_lowpass;
	a0 = 1.0;
	a1 = a2 = b1 = b2 = 0.0;
	frequency = 20;
	Q = 0.707;
	peakGain = 0.0;
	//Dummy variables to set sin/cos pointers.
	double a = 4;
	double b = 4;
	sn = &a;
	cs = &b;
}

MyFilter::~MyFilter()
{
}

void MyFilter::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	this->sampleRate = sampleRate;
	this->bufferLength = samplesPerBlock;
	delayedSampleY1 = 0.0;
	delayedSampleX1 = 0.0;
	delayedSampleY2 = 0.0;
	delayedSampleX2 = 0.0;
}

//Taken from episode 27 The Audio Programmer
//FIR
float MyFilter::simpleFIRHiPass(float currentSample, float sliderValue)
{
	//design equation a0 = a1 - 1.0
	float a0 = sliderValue - 1.0;
	float previousSample = delayedSampleX1;

	float processedSample = a0 * currentSample + sliderValue * previousSample;
	delayedSampleX1 = currentSample;
	return processedSample;
}

//Taken from https://en.wikipedia.org/wiki/Low-pass_filter
//IIR
float MyFilter::simpleIIRLowPass(float currentSample, float sliderValue)
{
	float previousSample = delayedSampleY1;

	//Should be same thing, Test and remove top if it is
	//float processedSample = (sliderValue * currentSample) + ((1-sliderValue) * previousSample);
	float processedSample = previousSample + sliderValue * (currentSample - previousSample);
	delayedSampleY1 = processedSample;
	return processedSample;
}

void MyFilter::setBiquad(int type, double Fc, double Q, double peakGainDB) {
	this->type = type;
	this->Q = Q;
	this->frequency = Fc;
	this->peakGain = peakGainDB;
	calcBiquad();
}

void MyFilter::setButterworth(int type, double Fc, double Q, double peakGainDB) {
	this->type = type;
	this->Q = Q;
	this->frequency = Fc;
	this->peakGain = peakGainDB;
	calcButterworth();
}

//Taken from https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
//Biquad filter Coefficients
void MyFilter::calcBiquad(void) {
	double norm;
	double V = pow(10, fabs(peakGain) / 20.0);
	double K = tan(M_PI * (frequency / sampleRate));
	switch (this->type) {
	case type_lowpass:
		norm = 1 / (1 + K / Q + K * K);
		a0 = K * K * norm;
		a1 = 2 * a0;
		a2 = a0;
		b1 = 2 * (K * K - 1) * norm;
		b2 = (1 - K / Q + K * K) * norm;
		break;

	case type_highpass:
		norm = 1 / (1 + K / Q + K * K);
		a0 = 1 * norm;
		a1 = -2 * a0;
		a2 = a0;
		b1 = 2 * (K * K - 1) * norm;
		b2 = (1 - K / Q + K * K) * norm;
		break;

	case type_bandpass:
		norm = 1 / (1 + K / Q + K * K);
		a0 = K / Q * norm;
		a1 = 0;
		a2 = -a0;
		b1 = 2 * (K * K - 1) * norm;
		b2 = (1 - K / Q + K * K) * norm;
		break;

	case type_notch:
		norm = 1 / (1 + K / Q + K * K);
		a0 = (1 + K * K) * norm;
		a1 = 2 * (K * K - 1) * norm;
		a2 = a0;
		b1 = a1;
		b2 = (1 - K / Q + K * K) * norm;
		break;

	case type_peak:
		if (peakGain >= 0) {    // boost
			norm = 1 / (1 + 1 / Q * K + K * K);
			a0 = (1 + V / Q * K + K * K) * norm;
			a1 = 2 * (K * K - 1) * norm;
			a2 = (1 - V / Q * K + K * K) * norm;
			b1 = a1;
			b2 = (1 - 1 / Q * K + K * K) * norm;
		}
		else {    // cut
			norm = 1 / (1 + V / Q * K + K * K);
			a0 = (1 + 1 / Q * K + K * K) * norm;
			a1 = 2 * (K * K - 1) * norm;
			a2 = (1 - 1 / Q * K + K * K) * norm;
			b1 = a1;
			b2 = (1 - V / Q * K + K * K) * norm;
		}
		break;
	case type_lowshelf:
		if (peakGain >= 0) {    // boost
			norm = 1 / (1 + sqrt(2) * K + K * K);
			a0 = (1 + sqrt(2 * V) * K + V * K * K) * norm;
			a1 = 2 * (V * K * K - 1) * norm;
			a2 = (1 - sqrt(2 * V) * K + V * K * K) * norm;
			b1 = 2 * (K * K - 1) * norm;
			b2 = (1 - sqrt(2) * K + K * K) * norm;
		}
		else {    // cut
			norm = 1 / (1 + sqrt(2 * V) * K + V * K * K);
			a0 = (1 + sqrt(2) * K + K * K) * norm;
			a1 = 2 * (K * K - 1) * norm;
			a2 = (1 - sqrt(2) * K + K * K) * norm;
			b1 = 2 * (V * K * K - 1) * norm;
			b2 = (1 - sqrt(2 * V) * K + V * K * K) * norm;
		}
		break;
	case type_highshelf:
		if (peakGain >= 0) {    // boost
			norm = 1 / (1 + sqrt(2) * K + K * K);
			a0 = (V + sqrt(2 * V) * K + K * K) * norm;
			a1 = 2 * (K * K - V) * norm;
			a2 = (V - sqrt(2 * V) * K + K * K) * norm;
			b1 = 2 * (K * K - 1) * norm;
			b2 = (1 - sqrt(2) * K + K * K) * norm;
		}
		else {    // cut
			norm = 1 / (V + sqrt(2 * V) * K + K * K);
			a0 = (1 + sqrt(2) * K + K * K) * norm;
			a1 = 2 * (K * K - 1) * norm;
			a2 = (1 - sqrt(2) * K + K * K) * norm;
			b1 = 2 * (K * K - V) * norm;
			b2 = (V - sqrt(2 * V) * K + K * K) * norm;
		}
		break;
	}

	return;
}

void MyFilter::calcButterworth(void)
{
	double norm;
	double freqT = 2 * tan(frequency * M_PI / sampleRate);
	double V = pow(10, fabs(peakGain) / 20.0);
	switch (this->type)
	{
	case type_lowpass:
		a0 = (freqT * freqT) / (4 + (2 * sqrt(2) * freqT) + freqT * freqT);
		a1 = 2 * a0;
		a2 = a0;
		b1 = ((2 * freqT * freqT) - 8) / (4 + (2 * sqrt(2) * freqT) + freqT * freqT);
		b2 = (4 - (2 * sqrt(2) * freqT) + freqT * freqT) / (4 + (2 * sqrt(2) * freqT) + freqT * freqT);
		break;
	case type_highpass:
		norm = freqT * freqT / 4 + freqT / sqrt(2) + 1;
		a0 = 1 / norm;
		a1 = -2 * a0;
		a2 = a0;
		b1 = ((freqT * freqT) / 2 - 2) / norm;
		b2 = ((freqT * freqT) / 4 - (freqT / sqrt(2)) + 1) / norm;
		break;
	}

	
}


//Taken from https://en.wikipedia.org/wiki/High-pass_filter
//IIR - DOES NOT WORK PROPERLY, NO SETTING OF FREQUENCY
float MyFilter::simpleIIRHiPass(float currentSample, float sliderValue)
{
	float a0 = sliderValue - 1.0;
	float previousSampleX = delayedSampleX1;
	float previousSampleY = delayedSampleY1;

	float processedSample = a0 * (previousSampleY + currentSample - previousSampleX);
	delayedSampleY1 = processedSample;
	delayedSampleX1 = currentSample;
	return processedSample;
}



