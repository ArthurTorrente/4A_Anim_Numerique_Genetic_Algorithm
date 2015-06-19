#ifndef _LIFECAPACITY_HPP_
#define _LIFECAPACITY_HPP_
#include "Capacity.hpp"


//Class representing the life capacity of an unit
class LifeCapacity : public Capacity {
private:
    float maxLife_;

    void updateValue_()
    {
        maxLife_ = value_ = 10*(1+level_);
    }

public:
    LifeCapacity():Capacity(0)
    {
        updateValue_();
    }

    //Decrease the life value
    bool takeDamage(float damageValue)
    {
        value_ -= damageValue;
        return value_ > 0;
    }

    //Return true if still have life, false otherwise
    bool isAlive() const
    {
        return value_ > 0;
    }

    //Increase the life value
    bool takeRegen(float regenValue)
    {
        value_ += regenValue;
        if(value_>maxLife_)value_ = maxLife_;
    }


};
#endif