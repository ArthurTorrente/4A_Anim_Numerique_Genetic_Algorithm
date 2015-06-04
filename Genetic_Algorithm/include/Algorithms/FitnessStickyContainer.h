#ifndef _FITNESSSTICKYCONTAINER_H_
#define _FITNESSSTICKYCONTAINER_H_

#include <memory>

#include "Stixel.h"

struct FitnessStickyContainer
{
    FitnessStickyContainer(const Stixel* s, unsigned int _fitness = 0)
        : stixel(s),
        fitness(_fitness)
    {}
    
    const Stixel* stixel;
    unsigned int fitness;

    bool operator<(const FitnessStickyContainer& f)
    {
        return fitness > f.fitness;
    }
};

#endif //_FITNESSSTICKYCONTAINER_H_