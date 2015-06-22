#ifndef _RANGECAPACITY_HPP_
#define _RANGECAPACITY_HPP_
#include "Capacity.hpp"


//Class representing the range capacity of an unit
class RangeCapacity : public Capacity {
private:

    void updateValue_()
    {
        value_ =10+level_*2.0f;
    }

public:
    RangeCapacity():Capacity(0)
    {
        updateValue_();
    }
};
#endif