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

void testBypassSilence()
{
	std::vector<double> inputData(128, 0.0);
	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, 128);

	Effects::Bypass(inFrame, outFrame);

	if(nearlyEqual(outFrame.getData(), inputData))
		std::cout << "Bypass Silence Equality Test " << GREEN << "PASSED\n" << CLEAR;
	else
		std::cout << "Bypass Silence Equality Test " << RED << "FAILED\n" << CLEAR;
}

void testBypassSine()
{
	std::vector<double> inputData(128, 0.0);
	for(int i = 0; i < inputData.size(); ++i)
	{
		inputData[i] = sin(i * 0.5);
	}

	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, 128);

	Effects::Bypass(inFrame, outFrame);

	if(nearlyEqual(outFrame.getData(), inputData))
		std::cout << "Bypass Sine Equality Test " << GREEN << "PASSED\n" << CLEAR;
	else
		std::cout << "Bypass Sine Equality Test " << RED << "FAILED\n" << CLEAR;
}

void testBypassExtremeValues()
{
	std::vector<double> inputData = {-123.123, 23122, 3585, 3, 0};
	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, inputData.size());

	Effects::Bypass(inFrame, outFrame);


	if(nearlyEqual(outFrame.getData(), inputData))
		std::cout << "Bypass Extremes Equality Test " << GREEN << "PASSED\n" << CLEAR;
	else
		std::cout << "Bypass Extremes Equality Test " << RED << "FAILED\n" << CLEAR;
}

void testBypassInputUntouched()
{
	std::vector<double> inputData(128, 0.0);
	for(int i = 0; i < inputData.size(); ++i)
	{
		inputData[i] = sin(i * 0.5);
	}

	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(44100, 128);

	Effects::Bypass(inFrame, outFrame);

	if(nearlyEqual(inFrame.getData(), inputData))
		std::cout << "Bypass Untouched Input Test " << GREEN << "PASSED\n" << CLEAR;
	else
		std::cout << "Bypass Untouched Input Test " << RED << "FAILED\n" << CLEAR;
}

void testBypassOutputOverwrite()
{
	std::vector<double> inputData(128, 0.0);
	for(int i = 0; i < inputData.size(); ++i)
	{
		inputData[i] = sin(i * 0.5);
	}

	std::vector<double> origOutputData(128, 1.0);

	AudioFrame inFrame(inputData, 44100);
	AudioFrame outFrame(origOutputData, 44100);

	Effects::Bypass(inFrame, outFrame);

	if(nearlyEqual(outFrame.getData(), inputData))
		std::cout << "Bypass Overrite Output Test " << GREEN << "PASSED\n" << CLEAR;
	else
		std::cout << "Bypass Overwrite Input Test " << RED << "FAILED\n" << CLEAR;
}

int main(void)
{
	testBypassSilence();
	testBypassSine();
	testBypassExtremeValues();
	testBypassInputUntouched();
	testBypassOutputOverwrite();

	return 0;
}
