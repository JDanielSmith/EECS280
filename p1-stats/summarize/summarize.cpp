// summarize.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <assert.h>
#include <stdlib.h>

#include <vector>
#include <stdexcept>
#include <map>
#include <utility> // std::pair
#include <algorithm>

std::vector<double> sort(std::vector<double> v)
{
    std::sort(v.begin(), v.end());
    return v;
}

template<typename TKey, typename TValue>
auto find(std::vector<std::pair<TKey, TValue>>& map, const TKey& key)
{
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        if (it->first == key)
        {
            return it;
        }
    }
    return map.end();
}

template<typename TKey, typename TValue>
TValue& index(std::vector<std::pair<TKey, TValue>>& map, const TKey& key)
{
    const auto it = find(map, key);
    if (it != map.end())
    {
        return it->second;
    }

    map.push_back({ key, TValue{} });
    return find(map, key)->second;
}


//REQUIRES: v is not empty
//EFFECTS: returns a summary of the dataset as (value, frequency) pairs
//  In the returned vector-of-pairs, the inner item is a (value, frequency)
//  pair.  The outer vector contains many of these pairs.  The pairs should be
//  sorted by value.
auto summarize_jds(std::vector<double> v)
{
    if (v.empty())
    {
        throw std::invalid_argument("v must not be empty.");
    }
    v = sort(v);

    std::vector<std::pair<double, int>> retval;
    for (const auto& value : v)
    {
        const auto it = find(retval, value);
        if (it == retval.end())
        {
            index(retval, value) = 0;
        }
        index(retval, value)++;
    }

    return retval; 
}

static bool adjust_freq(const double* pPrev, const double* pCurrent,
    int& freq, std::pair<double, int>& value_and_freq)
{
    if (pPrev == nullptr)
    {
        return false;
    }

    if (pCurrent != nullptr)
    {
        if (*pPrev == *pCurrent)
        {
            freq++;
            return false;
        }
    }

    value_and_freq = std::make_pair(*pPrev, freq);
    freq = 1;
    return true;
}
std::vector<std::pair<double, int>> summarize2(std::vector<double> v)
{
    v = sort(v);

    std::vector<std::pair<double, int>> retval;

    int freq = 1;
    for (size_t i = 0; i <= v.size(); i++)
    {
        const double* const pPrev = i == 0 ? nullptr : &(v[i - 1]);
        const double* const pCurrent = i >= v.size() ? nullptr : &(v[i]);

        std::pair<double, int> value_and_freq;
        if (adjust_freq(pPrev, pCurrent, freq, value_and_freq))
        {
            retval.push_back(value_and_freq);
        }
    }

    return retval;
}


std::vector<std::pair<double, int>> summarize(std::vector<double> v)
{
    std::vector<std::pair<double, int>> info;
    v = sort(v);
    for (size_t i = 0; i < v.size(); ++i)
    {
        int freq = 0;
        auto j = i;
        while (v[i] == v[j])
        {

            ++freq;
            j++;
            if (j == v.size()) {
                break;
            }

        }
        if (i != 0) {
            if (v[i] > v[i - 1]) {
                info.push_back({ v[i], freq });
            }
        }
        if (i == 0) {
            info.push_back({ v[0], freq });

        }

    }
    return info;
}


int main()
{
    //  {
    //    {1, 2},
    //    {2, 3},
    //    {17, 1}
    //  }
    //
    // This means that the value 1 occurred twice, the value 2 occurred three times,
    // and the value 17 occurred once
    const std::vector<double> input{ 17.0, 2.0, 1.0, 2.0, 2.0, 1.0 };
    const auto result = summarize2(input);

    assert(result.size() == 3);
    assert((result[0].first == 1.0) && (result[0].second == 2));
    assert((result[1].first == 2.0) && (result[1].second == 3));
    assert((result[2].first == 17.0) && (result[2].second == 1));

    return EXIT_SUCCESS;
}

