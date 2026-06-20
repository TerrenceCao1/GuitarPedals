#include <iostream>
#include "AudioFrame.h"
#include "Effects.h"

void Effects::Bypass(const AudioFrame &inFrame, AudioFrame &outFrame)
{
	outFrame.getDataMutable() = inFrame.getData();
}
