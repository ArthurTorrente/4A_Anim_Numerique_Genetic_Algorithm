#ifndef _ACTION_HPP_
#define _ACTION_HPP_

//Interface representing action returned by AI
class Action {
public:
    //Run the action
    //Log parameter indicate if we write something or not on the standard output
    virtual void execute(bool log = false) = 0;
};


#endif