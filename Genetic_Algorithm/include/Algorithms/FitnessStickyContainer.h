#ifndef _FITNESSSTICKYCONTAINER_H_
#define _FITNESSSTICKYCONTAINER_H_

#include <memory>
#include "Sticky.h"

struct FitnessStickyContainer
{
    unsigned int fitness;
    Sticky* sticky;
};

#endif //_FITNESSSTICKYCONTAINER_H_