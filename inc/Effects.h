/* Effects.h
 * Header File for a bunch of guitar pedal effects!
 *
 * TODO:
 *	-Bypass
 *	-Boost
 *	-Overdrive/Distort
 *	-Delay
 *	-Chorus
 *	-Reverb
 *	-Tremolo
 *	-Tuner
 *	-Looper
 * */

#pragma once
#include "AudioFrame.h"
#include <iostream>
#include <cmath>

namespace Effects 
{
	void Bypass(const AudioFrame& inFrame, AudioFrame& outFrame);
}

