#include <iostream>
#include <cassert>
#include "AudioFrame.h"
#include "Effects.h"

/*Test:
 * Equality (silence, sine, extreme values)
 * Input untouched
 * Output touched lol
*/

#define RED "\033[31m"
#define GREEN "\033[32m"
#define CLEAR "\033[0m"

bool nearlyEqual(const std::vector<double>& a, const std::vector<double>& b, double epsilon = 1e-9)
{
	if(a.size() != b.size()) return false;
	for(size_t i = 0; i < a.size(); ++i)
	{
		if(std::abs(a[i] - b[i]) > epsilon) return false;
	}

	return true;
}

void testBoostZeroDb()
{
	std::vector<double> inputData = {0.1, -0.345, 0.1, -90.34};

	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, inputData.size());

	Effects::Boost(inFrame, outFrame, 0.0);

	if(nearlyEqual(outFrame.getData(), inputData))
		std::cout << "Boost Zero Db Test " << GREEN << "PASSED\n" << CLEAR;
	else
		std::cout << "Boost Zero Db Test " << RED << "FAILED\n" << CLEAR;
}

void testBoost6DbDoubles()
{
	std::vector<double> inputData = {0.1, -0.345, 0.1, -90.34};

	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, inputData.size());

	Effects::Boost(inFrame, outFrame, 6.0);

	std::vector<double> twiceInput(inputData.size());
	for(double sample : inputData)
		twiceInput.push_back(2 * sample);

	if(nearlyEqual(outFrame.getData(), twiceInput, 0.5))
		std::cout << "Boost 6Db Doubles Test " << GREEN << "PASSED\n" << CLEAR;
	else
		std::cout << "Boost 6Db Doubles Test " << RED << "FAILED\n" << CLEAR;
}

void testBoostPreservesSign()
{
	std::vector<double> inputData = {5.0, -5.0, 0};
	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, inputData.size());

	Effects::Boost(inFrame, outFrame, 12.0);

	const auto& out = outFrame.getData();

	if(out[0] < 0)
	{
		std::cout << "Boost Preserves Sign Test " << RED << "FAILED\n" << CLEAR;
		return;
	}
	if(out[1] > 0)
	{
		std::cout << "Boost Preserves Sign Test " << RED << "FAILED\n" << CLEAR;
		return;
	}

	std::cout << "Boost Preserves Sign Test " << GREEN << "PASSED\n" << CLEAR;
}

void testBoostNegativeDbAttenuates()
{
	std::vector<double> inputData = {0.5, -0.5};
	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, inputData.size());

	Effects::Boost(inFrame, outFrame, -6.0);

	const auto& out = outFrame.getData();
	if(std::abs(out[0]) > std::abs(inputData[0]))
	{
		std::cout << "Boost Negative Db Attentuates Test " << RED << "FAILED\n" << CLEAR;
		return;
	}
	if(std::abs(out[1]) > std::abs(inputData[1]))
	{
		std::cout << "Boost Negative Db Attentuates Test " << RED << "FAILED\n" << CLEAR;
		return;
	}

	std::cout << "Boost Negative Db Attentuates Test " << GREEN << "PASSED\n" << CLEAR;
}

void testBoostInputUnmodified()
{
	std::vector<double> inputData = {0.1, -0.345, 0.1, -90.34};
	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, inputData.size());

	Effects::Boost(inFrame, outFrame, 6.0);
	if(!nearlyEqual(inputData, inFrame.getData()))
	{
		std::cout << "Boost Input Unmodified Test " << RED << "FAILED\n" << CLEAR;
		return;
	}
	std::cout << "Boost Input Unmodified Test " << GREEN << "PASSED\n" << CLEAR;
}

int main(void)
{
	testBoostZeroDb();
	testBoost6DbDoubles();
	testBoostPreservesSign();
	testBoostNegativeDbAttenuates();
	testBoostInputUnmodified();

	return 0;
}
