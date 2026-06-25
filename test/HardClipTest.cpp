#include <iostream>
#include <cassert>
#include "AudioFrame.h"
#include "Effects.h"

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


