#ifndef _EMPTY_ACTION_HPP_
#define _EMPTY_ACTION_HPP_
#include "Unit.hpp"
#include "Action.hpp"
#include "Point.hpp"
#include <iostream>

//Class representing an action which do nothing
class EmptyAction : public Action {
private:
    Unit* unit_;

public:

    //Constructor
    EmptyAction(Unit& unit):unit_(&unit) {}

    //Run the action
    //Log parameter indicate if we write something or not on the standard output
    void execute(bool log = false)
    {
        if(log)std::cout<<"Unit "<<unit_->getId()<<" do nothing..."<<std::endl;

    }
};

#endif