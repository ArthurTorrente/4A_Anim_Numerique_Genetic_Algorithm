#ifndef _FIRERATECAPACITY_HPP_
#define _FIRERATECAPACITY_HPP_
#include "Capacity.hpp"



//Class representing the firerate capacity of an unit
class FirerateCapacity : public Capacity {
private:
    float reloadingTime_;

    void updateValue_()
    {
        value_ = 0;
        reloadingTime_ = 1000.0/(level_+1.0);
    }

public:
    FirerateCapacity():Capacity(0)
    {
        updateValue_();
    }

    //set the cooldown to the maximum
    void shoot()
    {
        value_ = reloadingTime_;
    }

    //return true if the cooldown is at 0, false otherwise
    bool canShoot() const
    {
        return value_ < 0.001;
    }

    //decrease the cooldown
    void reload()
    {
        value_ -= 1.0;
        if(value_ < 0)value_ = 0.0;
    }

};
#endif