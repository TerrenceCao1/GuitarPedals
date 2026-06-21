#include <iostream>
#include "AudioFrame.h"
#include "Effects.h"

void Effects::Bypass(const AudioFrame &inFrame, AudioFrame &outFrame)
{
	outFrame.getDataMutable() = inFrame.getData();
}

void Effects::Boost(const AudioFrame &inFrame, AudioFrame &outFrame, double gainDb)
{
	double linearGain = std::pow(10.0, gainDb / 20.0);
	const auto& in = inFrame.getData();
	auto& out = outFrame.getDataMutable();

	for(size_t i = 0; i < in.size(); i++)
	{
		out[i] = in[i] * linearGain;
	}
}
