// sum.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <assert.h>

#include <iostream>
#include <vector>
#include <cmath>

double sum(std::vector<double> v)
{
    double totsum = 0.0;
    for (auto value : v)
    {
        totsum += value;
    }
    return totsum;
}

static bool double_eq(double lhs, double rhs, double epsilon = 0.0001)
{
    return std::abs(lhs - rhs) <= epsilon;
}

void test_sum_testdata_set(std::vector<double> input, double expected)
{
    std::cout << "test_sum_testdata_set\n";

    const auto actual = sum(input);
    //assert(actual == expected);
    assert(double_eq(actual, expected));
}



int main()
{
    const std::vector<double> testdata2 = { 1.25, 5.3, 2.5, 4.4, 3.25, 3.7, 2.3, 0.3 };
    test_sum_testdata_set(testdata2, 23.0);
}
