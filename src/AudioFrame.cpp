/*
 * effects.cpp
 * Source file for effects.h
 *
 * */
#include <iostream>
#include "AudioFrame.h"

// AudioFrame Constructors -------------------------------------------
AudioFrame::AudioFrame()
	: data(), dataSize(0), samplingRate(0), numChannels(1)
{}

// Empty constructor
AudioFrame::AudioFrame(size_t samplingRate, size_t numSamples)
	: data(numSamples, 0.0), dataSize(numSamples), samplingRate(samplingRate), numChannels(1)
{}

// From vector
AudioFrame::AudioFrame(std::vector<double> data, size_t samplingRate)
	: data(data), dataSize(data.size()), samplingRate(samplingRate), numChannels(1)
{}

// From Raw DMA Buffer
AudioFrame::AudioFrame(const double* rawBuffer, size_t numSamples, size_t samplingRate)
	: dataSize(numSamples), samplingRate(samplingRate), numChannels(1)
{
	for(size_t i = 0; i < numSamples; i++)
	{
		// rawBuffer from DMA is interleaved 16bit [L0, R0, L1, R1 ...]
		data[i] = rawBuffer[i * 2] / 32768.0; // only left channel
	}
}

// Copy Constructor
AudioFrame::AudioFrame(const AudioFrame& copy)
	: data(copy.data), dataSize(copy.dataSize), samplingRate(copy.samplingRate), numChannels(copy.numChannels)
{}


// AudioFrame Destructor ----------------------------------------------
AudioFrame::~AudioFrame()
{
	data.clear();
}

// Access Functions ---------------------------------------------------
size_t AudioFrame::getSamplingRate() const
{
	return samplingRate;
}

size_t AudioFrame::getNumSamples() const
{
	return dataSize;
}

size_t AudioFrame::getNumChannels() const
{
	return numChannels;
}

double AudioFrame::getDuration() const
{
	return duration;
}


