#ifndef _SPEEDCAPACITY_HPP_
#define _SPEEDCAPACITY_HPP_
#include "Capacity.hpp"

//Class representing the speed capactiy of an unit
class SpeedCapacity : public Capacity {
private:

    void updateValue_()
    {
        value_ = 1+level_;
    }

public:
    SpeedCapacity():Capacity(0)
    {
        updateValue_();
    }
};
#endif