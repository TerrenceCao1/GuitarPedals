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
	/*
	 * @brief	Adds no effects, just copies the inFrame to the outFrame
	 * @param	inFrame		input audio data frame
	 * @param	outFrame	output audio data frame
	 * @return	outputFrame gets filled
	 */
	void Bypass(const AudioFrame& inFrame, AudioFrame& outFrame);

	/*
	 * @brief	Increases volume without any other effects
	 * @param	inFrame		input audio data frame
	 * @param	outFrame	output audio data frame
	 * @param	gainDb		how much volume increase
	 * @return	outputFrame gets filled with boosted values
	 */
	void Boost(const AudioFrame& inFrame, AudioFrame& outFrame, double gainDb);

	/*
	 * @brief	Slices the output wave at the threshold amplitude
	 * @param	inFrame		input audio data frame
	 * @param	outFrame	output audio data frame
	 * @param	gain		how much volume increase
	 * @param	threshold	cutting off point for +-
	 * @return	outputFrame gets hardClipped lol
	 */
	void HardClip(const AudioFrame& inFrame, AudioFrame& outFrame, double gain, double threshold = 0.5);

	/*
	 * @brief	Applies tanh function to smooth out waveform 
	 * @param	inFrame		input audio data frame
	 * @param	outFrame	output audio data frame
	 * @param	gain		how much volume increase
	 * @return	outputFrame gets softclipped
	 */
	void SoftClip(const AudioFrame& inFrame, AudioFrame& outFrame, double gain);

	/*
	 * @brief	Applies Distortion to audio
	 * @param	inFrame		input audio data frame
	 * @param	outFrame	output audio data frame
	 * @param	gain		how much volume increase
	 * @param	threshold	how hardclipped
	 * @return	outputFrame gets distorted inFrame
	 */
	void Distort(const AudioFrame& inFrame, AudioFrame& outFrame, double gain, double threshold = 0.5);

	/*
	 * @brief	Applies Overdrive to audio
	 * @param	inFrame		input audio data frame
	 * @param	outFrame	output audio data frame
	 * @param	gain		how much volume increase
	 * @return	outputFrame gets overdrived inFrame
	 */
	void Overdrive(const AudioFrame& inFrame, AudioFrame& outFrame, double gain);


}

