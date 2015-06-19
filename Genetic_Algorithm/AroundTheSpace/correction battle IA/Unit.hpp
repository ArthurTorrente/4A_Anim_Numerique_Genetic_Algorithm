#ifndef _UNIT_HPP_
#define _UNIT_HPP_
#include <memory>
#include <vector>
#include <string>
#include <ostream>
#include <istream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include "Capacity.hpp"
#include "SpeedCapacity.hpp"
#include "LifeCapacity.hpp"
#include "RegenCapacity.hpp"
#include "DamageCapacity.hpp"
#include "FirerateCapacity.hpp"
#include "RangeCapacity.hpp"
#include "ArmorCapacity.hpp"
#include "Point.hpp"



//Class representing a unit, i.e. element of an army
class Unit {

private:

    //static counter used for unique id creation
    static int idCount_;

    //unique id of the Unit
    int id_;

    //code representing the AI used by the unit
    std::string iaCode_;

    //vector of the unit's capacities
    //order is : speed, life, armor, regen, damage, range, firerate
    std::vector<std::unique_ptr<Capacity> > capacities_;

    //position of the unit
    Point position_;


    //Method in charge of the initialization of id, position(random), and capacities
    void init_();

public:

    //Constructor : global level will be randomly dispatched among the capacities
    Unit(int globalLevel);

    //Constructor from the code of AI and the level of the different capacities
    Unit(std::string iaCode, int speedLevel, int lifeLevel, int armorLevel, int regenLevel, int damageLevel, int rangeLevel, int firerateLevel);

    //Constructor from the code of AI and the level of the 7 capacities
    Unit(std::string iaCode, std::vector<int>& levels);

    //Copy constructor
    Unit(const Unit& unit);

    //Assignment operator overload
    Unit& operator=(Unit unit);

    //Swap the content of the parameter and the current object
    void swap(Unit& unit);

    //Provide the unique identifier of the unit
    int getId()const
    {
        return id_;
    }

    //provide the AI code of the unit
    std::string getIACode()const
    {
        return iaCode_;
    }

    //Provide the speed capacity of the unit
    SpeedCapacity& getSpeed()
    {
        return static_cast<SpeedCapacity&>(*(capacities_[0]));
    }

    //Provide the life capacity of the unit
    LifeCapacity& getLife()
    {
        return static_cast<LifeCapacity&>(*(capacities_[1]));
    }

    //Provide the armor capacity of the unit
    ArmorCapacity& getArmor()
    {
        return static_cast<ArmorCapacity&>(*(capacities_[2]));
    }

    //Provide the regen capacity of the unit
    RegenCapacity& getRegen()
    {
        return static_cast<RegenCapacity&>(*(capacities_[3]));
    }

    //Provide the damage capacity of the unit
    DamageCapacity& getDamage()
    {
        return static_cast<DamageCapacity&>(*(capacities_[4]));
    }

    //Provide the range capacity of the unit
    RangeCapacity& getRange()
    {
        return static_cast<RangeCapacity&>(*(capacities_[5]));
    }

    //Provide the firerate capacity of the unit
    FirerateCapacity& getFirerate()
    {
        return static_cast<FirerateCapacity&>(*(capacities_[6]));
    }

    //Provide the capacity with the given index
    Capacity* getCapacity(int index)
    {
        return capacities_.at(index).get();
    }

    //Provide the capacity with the given index
    const Capacity* getCapacity(int index) const
    {
        return capacities_.at(index).get();
    }

    //Provide the position of the unit
    Point getPosition()const
    {
        return position_;
    }

    //Change the position of the unit by replacing the existing one
    void setPosition(const Point& position)
    {
        position_ = position;
    }


    //Change the position of the unit by moving in direction of the new one
    void moveToPosition(const Point& position)
    {
        position_.moveTo(position, getSpeed().getValue());
    }

    //Provide the global level of the unit (I.E. the sum of all capacities levels)
    int getLevel()const;

    //Refresh the capacities of the unit, most often used at the beginning of fight turn.
    //Reduce the cooldown of the firerate, and apply the regen on the life
    void refresh();

    //Make the unit shot if it is possible. In this case, the cooldown
    //is set to maximum value and the method return true, otherwise, it return false.
    bool shoot();

    //Apply the damages to the unit, by first reducing them by the armor,
    //then apply it to the life.
    void takeDamage(float value);

    //Return true if the unit have still life, false otherwise.
    bool isAlive()const
    {
        return const_cast< Unit * const>(this)->getLife().isAlive();
    }

    //Provide a randomly mutated version of the current unit
    Unit mutate()const;

    //Star operator overloading, providing a random crossing of the current
    //unit and the one provided in parameter.
    Unit operator*(const Unit& unit)const;

    //Save the current unit in the given output stream.
    void save(std::ostream& out)const;

    //Load a unit from the input stream
    static Unit load(std::istream& in);


};

//Shift operator overloading, printing the unit in the output stream.
inline std::ostream& operator<<(std::ostream& out, const Unit& unit)
{
    out<<"Unit#"<<unit.getId()<<"["<<unit.getIACode();
    for(int i = 0; i < 7; i++)out<<"; "<<(unit).getCapacity(i)->getLevel();
    out<<"]";
    return out;
}

#endif