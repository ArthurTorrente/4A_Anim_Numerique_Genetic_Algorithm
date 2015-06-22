#ifndef _ARMORCAPACITY_HPP_
#define _ARMORCAPACITY_HPP_
#include "Capacity.hpp"



//Class representing the armor capacity of an unit
class ArmorCapacity : public Capacity {
private:

    void updateValue_()
    {
        value_ =level_*0.5f;
    }

public:
    ArmorCapacity():Capacity(0)
    {
        updateValue_();
    }
};
#endif