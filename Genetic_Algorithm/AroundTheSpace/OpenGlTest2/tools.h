#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <algorithm>

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

    StixelsWrapper( unsigned int f)
        :
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

    unsigned int fitness;
};

#endif //_TOOLS_H_