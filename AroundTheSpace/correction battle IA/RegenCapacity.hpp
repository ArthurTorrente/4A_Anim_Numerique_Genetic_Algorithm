#ifndef _REGENCAPACITY_HPP_
#define _REGENCAPACITY_HPP_
#include "Capacity.hpp"


//Class representing the regen capactiy of an unit
class RegenCapacity : public Capacity {
private:

    void updateValue_()
    {
        value_ =level_*0.01f;
    }

public:
    RegenCapacity():Capacity(0)
    {
        updateValue_();
    }
};
#endif