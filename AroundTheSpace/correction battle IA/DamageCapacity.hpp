#ifndef _DAMAGECAPACITY_HPP_
#define _DAMAGECAPACITY_HPP_
#include "Capacity.hpp"



//Class representing the damage capacity of an unit
class DamageCapacity : public Capacity {
private:

    void updateValue_()
    {
        value_ =(1+level_)*1.5f;
    }

public:
    DamageCapacity():Capacity(0)
    {
        updateValue_();
    }
};
#endif