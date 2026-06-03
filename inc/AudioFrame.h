/* effects.h
 * Header File for a bunch of guitar pedal effects!
 *
 * TODO:
 * - Distortion Effects:
 *		- Hard Clip 
 *		- Soft Clip
 *		- Fuzz
 * - Delay
 * - Boost
 * - Tremolo
 * */

#include <cstddef>
#include <vector>
#include <functional>
#include <cinttypes>
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

		// Manipulation Functions:
		void normalize();

		// Output Function to I2S:
		std::vector<int16_t> toInt16Buffer() const;


		// Operator Overloads:
		AudioFrame& operator=(const AudioFrame& copy);
	
	private:
		std::vector<double> data;
		size_t				dataSize;
		size_t				samplingRate;
		size_t				numChannels;
		double				duration;
};
