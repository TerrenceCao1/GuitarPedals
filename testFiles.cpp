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


int main(int argc, char** argv)
{
	const int numSamples = 10000;
	const double freq = 0.1;
	auto yData = createSinHelper(freq, numSamples);

	display(yData, numSamples, 50, "Sine Wave");
}
