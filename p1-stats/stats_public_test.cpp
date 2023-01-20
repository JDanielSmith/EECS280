// Project UID 5366c7e2b77742d5b2142097e51561a5
// THIS TEST CASE WILL ONLY TELL YOU IF YOUR CODE COMPILES.
// YOU NEED TO WRITE YOUR OWN COMPREHENSIVE TEST CASES IN stats_tests.cpp

#include "stats.h"
#include <iostream>
#include <cassert>
#include <vector>

int main()
{
	std::vector<double> data{ 1, 2, 3 };

	std::cout << "count" << count(data) << "\n";
	std::cout << "sum" << sum(data) << "\n";
	std::cout << "mean" << mean(data) << "\n";
	std::cout << "median" << median(data) << "\n";
	std::cout << "mode" << mode(data) << "\n";
	std::cout << "min" << min(data) << "\n";
	std::cout << "max" << max(data) << "\n";
	std::cout << "stdev" << stdev(data) << "\n";
	std::cout << "percentile" << percentile(data, 0.5) << "\n";

	summarize(data);

	return 0;
}

