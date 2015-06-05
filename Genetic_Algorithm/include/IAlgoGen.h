#ifndef _ALGOGEN_H_
#define _ALGOGEN_H_

#include <vector>

#include "cinder/gl/Texture.h"

#include "Stixel.h"

class IAlgoGen
{
public:
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
    virtual StixelsWrapper operator()(const std::vector<Stixel>&) const = 0;
};

#endif //_ALGOGEN_H_