/*
 * AudioFrame.cpp
 * Source file for AudioFrame.h
 *
 * */
#include <iostream>
#include "AudioFrame.h"

// AudioFrame Constructors -------------------------------------------
AudioFrame::AudioFrame()
	: data(), dataSize(0), samplingRate(0), numChannels(1)
{
	data.fill(0.0);
}

// Empty constructor
AudioFrame::AudioFrame(size_t samplingRate, size_t numSamples)
	: data(), dataSize(numSamples), samplingRate(samplingRate), numChannels(1)
{
	data.fill(0.0);
}

// From Raw DMA Buffer
AudioFrame::AudioFrame(const double* rawBuffer, size_t numSamples, size_t samplingRate)
	: data(), dataSize(numSamples), samplingRate(samplingRate), numChannels(1)
{
	for(size_t i = 0; i < numSamples && i < MAX_FRAME_SAMPLES; i++)
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

std::array<double, MAX_FRAME_SAMPLES>& AudioFrame::getDataMutable()
{
	return data;
}

const std::array<double, MAX_FRAME_SAMPLES>& AudioFrame::getData() const
{
	return data;
}

// Manipulation Functions ----------------------------------------------
double AudioFrame::normalize()
{
	double max = 0;
	for(double point : data)
	{
		double pointValue = std::abs(point);
		if(pointValue > max)
			max = pointValue;
	}

	for(int i = 0; i < data.size(); ++i)
		data[i] = data[i] / max;

	return max;
}

// Operator Overloads ---------------------------------------------------
AudioFrame& AudioFrame::operator=(const AudioFrame& copy)
{
	if (this == &copy)
		return *this;

	data = copy.data;
	dataSize = copy.dataSize;
	samplingRate = copy.samplingRate;
	numChannels = copy.numChannels;
	duration = copy.duration;

	return *this;
}
