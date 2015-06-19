#ifndef _CAPACITY_HPP_
#define _CAPACITY_HPP_



//Abstract class representing an aspect or capacity of an unit
class Capacity {
protected:
    //Level of the capacity
    int level_;
    //Value of the capacity
    float value_;

    //Method updating the value of the capacity in regard of the level
    virtual void updateValue_() = 0;

public:

    //Constructor from level
    Capacity(int level):level_(level) {}

    //Destructor
    virtual ~Capacity() {}

    //Increase the level of the capacity
    void upgrade(int nbLevels = 1)
    {
        level_+=nbLevels;
        updateValue_();
    }

    //Decrease the level of the capacity
    void downgrade(int nbLevels = 1)
    {
        level_-=nbLevels;
        if(level_<0)level_ = 0;
        updateValue_();
    }

    //Level getter
    int getLevel() const
    {
        return level_;
    }

    //Value getter
    float getValue() const
    {
        return value_;
    }
};
#endif