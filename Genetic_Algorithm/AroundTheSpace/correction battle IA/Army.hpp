#ifndef _ARMY_HPP_
#define _ARMY_HPP_
#include "Unit.hpp"
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <ostream>


//Class representing an army, i.e. a set of units
class Army {
private:
    //vector storing the units
    std::vector<std::unique_ptr<Unit> > units_;

    //Method for deep copying the units vector
    void copyUnits_(const std::vector<std::unique_ptr<Unit> >& units);

public:

    //Constructor with parameters the number of units, and the level of each
    Army(int size, int level);

    //Constructor from a vector of units, which will be deep copied
    Army(std::vector<std::unique_ptr<Unit> >& units);

    //Copy constructor
    Army(const Army& army);

    //Swap the content of the parameter and the current object
    void swap(Army& army);

    //Assignment operator overload
    Army& operator=(Army army);

    //Getter fot the units list
    std::vector<std::unique_ptr<Unit> >& getUnitsList()
    {
        return units_;
    }


    //Return the number of units
    int size()const
    {
        return units_.size();
    }

    //Return the unit with the given ID
    Unit& getUnit(int id)
    {
        auto it = std::find_if(units_.begin(), units_.end(), [id](const std::unique_ptr<Unit>& unit) {
            return unit->getId()==id;
        });
        if(it == units_.end())throw std::invalid_argument("wrong id : unit "+ std::to_string(id)+" not found");
        return *(it->get());
    }


    //Get the unit the closest from the given position
    Unit& getNearestUnit(const Point& p);

    //Get the unit the furthest from the given position
    Unit& getFurthestUnit(const Point& p);


    //Get the unit with the lowest level for the given capacity
    Unit& getLowestUnit(int capa_index);


    //Get the unit with the highest level for the given capacity
    Unit& getHigestUnit(int capa_index);

    //Remove all the units with non positive life value
    void purge();

    //Provide a randomly mutated version of the current army
    Army mutate()const;

    //Star operator overloading, providing a random crossing of the current
    //army and the one provided in parameter.
    Army operator*(const Army& army)const;

    //Save the current army in the given output stream
    void save(std::ostream& out)const;

    //Load an army from the input stream
    static Army load(std::istream& in);


};

//Shift operator overloading, printing the unit in the output stream.
inline std::ostream& operator<<(std::ostream& out, const Army& army)
{
    out<<"====================ARMY===================================="<<std::endl;
    std::vector<std::unique_ptr<Unit> >& units = const_cast<Army&>(army).getUnitsList();
    for(auto it = units.begin(); it != units.end(); ++it) {
        out<<**it<<std::endl;
    }
    out<<"===================/ARMY===================================="<<std::endl;

    return out;
}


#endif