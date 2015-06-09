#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <algorithm>
#include "Stixel.h"

namespace tools
{
    template<typename T>
    T clamp(T value, T min, T max)
    {
        return std::min(max, std::max(value, min));
    }
}


/* OLD VOIR SI ON GARDE */
struct StixelsWrapper
{
    StixelsWrapper()
        : fitness(0)
    {}

    StixelsWrapper(const std::vector<Stixel>& s, unsigned int f)
        : stixel(s),
        fitness(f)
    {}

    bool operator>(const StixelsWrapper& sw)
    {
        return sw.fitness < fitness;
    }

    bool operator<(const StixelsWrapper& sw)
    {
        return fitness > sw.fitness;
    }

    std::vector<Stixel> stixel;
    unsigned int fitness;
};

#endif //_TOOLS_H_