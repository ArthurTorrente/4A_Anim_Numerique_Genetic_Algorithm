#ifndef _AI_HPP_
#define _AI_HPP_
#include <memory>
#include "Action.hpp"
#include "Unit.hpp"
#include "Army.hpp"
//class representing an Artificial Intelligence
class AI {
public:

    //Parenthesis overloading for applying the AI on the unit provided in parameter
    //Return the action which have to be done by the unit in this context
    std::unique_ptr<Action> operator()(Unit& unit, Army& allies, Army& opponents);
};


#endif