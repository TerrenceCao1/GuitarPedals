/* AudioFrame.h
 * Header File for Audio Frame ADT!
 *
 * Contains the function skeletons for a class for a frame of audio data
 * Each audio buffer will be BUFFER_SIZE long and will be dynamically allocated (only being allocated once)
 *
 * */

#pragma once
#include <cstddef>
#include <vector>
#include <cinttypes>

#define BUFFER_SIZE 128
class AudioFrame
{
	public:
		// Constructors
		AudioFrame();
		AudioFrame(const AudioFrame& copy);												// Copy Constructor
		AudioFrame(size_t samplingRate, size_t numSamples);								// Empty Constructor
		AudioFrame(std::vector<double> data, size_t samplingRate);						// From Existing Buffer
		AudioFrame(const double* rawBuffer, size_t numSamples, size_t samplingRate);	// From raw DMA buffer

		// Destructor
		~AudioFrame();

		// Access Functions:
		size_t getSamplingRate()	const;
		size_t getNumSamples()		const;
		size_t getNumChannels()		const;
		double getDuration()		const;

		std::vector<double>& getDataMutable();
		const std::vector<double>& getData() const;

		// Manipulation Functions:
		double normalize(); // Returns the max amplitude so that we can later reboost up to original

		// Output Function to I2S:
		std::vector<int16_t> toInt16Buffer() const;


		// Operator Overloads:
		AudioFrame& operator=(const AudioFrame& copy);
	
	private:

		// Data Fields:
		std::vector<double> data;
		size_t				dataSize = BUFFER_SIZE;
		size_t				samplingRate;
		size_t				numChannels;
		double				duration;
};

