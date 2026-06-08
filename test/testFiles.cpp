#include <cmath>
#include <cstddef>
#include <iostream>
#include <sciplot/Vec.hpp>
#include <sciplot/sciplot.hpp>


static std::vector<double> createSinHelper(double freq, int numSamples)
{
	std::vector<double> ret(numSamples);
	for(int i = 0; i < numSamples; i++)
	{
		ret[i] = std::sin(freq * i);
	}

	return ret;
}

static void display(std::vector<double> data, std::size_t dataSize, float xDomainSize, std::string label)
{
    sciplot::Vec x = sciplot::linspace(0.0, xDomainSize, xDomainSize * 10);
	sciplot::Vec y(data.data(), dataSize);

	sciplot::Plot2D plot1;
    plot1.palette("paired");
    plot1.drawCurve(x, y).label(label).lineWidth(4);

    sciplot::Figure figure = {{plot1}};
    sciplot::Canvas canvas = {{figure}};
    canvas.defaultPalette("set1");
    canvas.show();
}

// Hard Clipping removes waveform samples outside of a certain threshold
std::vector<double> hardClip(std::vector<double> input, double gain, double threshold = 0.5)
{
	std::vector<double> output(input.size());
	for(size_t i = 0; i < input.size(); ++i)
	{
		double amplified = input[i] * gain;
		output[i] = std::clamp(amplified, -threshold, threshold);
	}
	return output;
}

std::vector<double> softClip(std::vector<double> input, double gain)
{
	std::vector<double> output(input.size());
	for(size_t i = 0; i < input.size(); ++i)
	{
		output[i] = std::tanh(input[i] * gain);
	}

	return output;
}

static std::vector<double> distort(std::vector<double> input, double distortFactor)
{
	return input;
}

int main(int argc, char** argv)
{
	const int numSamples = 10000;
	const double freq = 0.1;
	auto yData = createSinHelper(freq, numSamples);
	auto hardClipped = hardClip(yData, 2, 1.5);
	auto softClipped = softClip(yData, 2);

	display(softClipped, numSamples, 50, "hardClipped");
}
