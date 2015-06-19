#include <cstdlib>
#include <algorithm>
#include "Unit.hpp"
#include <memory>

//static counter used for unique id creation
int Unit::idCount_=0;

//Method in charge of the initialization of id, position(random), and capacities
void Unit::init_()
{
    id_ = idCount_++;
    this->position_ = Point(std::rand()%100,std::rand()%100);
    this->capacities_.reserve(7);
    this->capacities_.push_back(std::unique_ptr<Capacity>(new SpeedCapacity()));
    this->capacities_.push_back(std::unique_ptr<Capacity>(new LifeCapacity()));
    this->capacities_.push_back(std::unique_ptr<Capacity>(new ArmorCapacity()));
    this->capacities_.push_back(std::unique_ptr<Capacity>(new RegenCapacity()));
    this->capacities_.push_back(std::unique_ptr<Capacity>(new DamageCapacity()));
    this->capacities_.push_back(std::unique_ptr<Capacity>(new RangeCapacity()));
    this->capacities_.push_back(std::unique_ptr<Capacity>(new FirerateCapacity()));
}


//Constructor : global level will be randomly dispatched among the capacities
Unit::Unit(int globalLevel)
{
    init_();
    if(std::rand()%2)this->iaCode_ = "L";
    else this->iaCode_ = "H";
    if(rand()%8==0)this->iaCode_ += "D";
    else this->iaCode_ += '0'+ (char)(rand()%7);
    while(globalLevel--) {
        this->capacities_[std::rand()%this->capacities_.size()]->upgrade();
    }
}


//Constructor from the code of AI and the level of the different capacities
Unit::Unit(std::string iaCode, int speedLevel, int lifeLevel, int armorLevel, int regenLevel, int damageLevel, int rangeLevel, int firerateLevel)
{
    init_();
    this->iaCode_ = iaCode;
    getSpeed().upgrade(speedLevel);
    getLife().upgrade(lifeLevel);
    getArmor().upgrade(armorLevel);
    getRegen().upgrade(regenLevel);
    getDamage().upgrade(damageLevel);
    getRange().upgrade(rangeLevel);
    getFirerate().upgrade(firerateLevel);
}


//Constructor from the code of AI and the level of the 7 capacities
Unit::Unit(std::string iaCode, std::vector<int>& levels)
{
    init_();
    this->iaCode_ = iaCode;
    for(int i = 0; i < levels.size() && i < capacities_.size(); ++i) {
        capacities_[i]->upgrade(levels[i]);
    }
}


//Copy constructor
Unit::Unit(const Unit& unit)
{
    init_();
    this->iaCode_ = unit.iaCode_;
    for(int i = 0;  i < capacities_.size(); ++i) {
        capacities_[i]->upgrade(unit.capacities_[i]->getLevel());
    }
}


//Swap the content of the parameter and the current object
void Unit::swap(Unit& unit)
{
    std::swap(capacities_, unit.capacities_);
    std::swap(iaCode_, unit.iaCode_);
    std::swap(position_, unit.position_);
    std::swap(id_, unit.id_);
}


//Assignment operator overload
Unit& Unit::operator=(Unit unit)
{
    swap(unit);
    return *this;
}


//Provide the global level of the unit (I.E. the sum of all capacities levels)
int Unit::getLevel()const
{
    return std::accumulate(capacities_.begin(), capacities_.end(),0,
    [](int& a,  const std::unique_ptr<Capacity>& b) {
        return a+b->getLevel();
    });
}


//Refresh the capacities of the unit, most often used at the beginning of fight turn.
//Reduce the cooldown of the firerate, and apply the regen on the life
void Unit::refresh()
{
    getLife().takeRegen(getRegen().getValue());
    getFirerate().reload();
}


//Make the unit shot if it is possible. In this case, the cooldown
//is set to maximum value and the method return true, otherwise, it return false.
bool Unit::shoot()
{
    if(getFirerate().canShoot()) {
        getFirerate().shoot();
        return true;
    }
    return false;
}


//Apply the damages to the unit, by first reducing them by the armor,
//then apply it to the life.
void Unit::takeDamage(float value)
{
    getLife().takeDamage(value);
}


//Provide a randomly muteted version of the current unit
Unit Unit::mutate()const
{
    int index = std::rand()%7;
    std::vector<int> levels(7);
    for(int i = 0; i < 7; ++i) {
        levels[i] = capacities_[i]->getLevel();
    }
    while(levels[index]==0) {
        index = std::rand()%7;
    }
    int index2 = std::rand()%7;
    while(index == index2) {
        index2 = std::rand()%7;
    }
    levels[index]--;
    levels[index2]++;
    return Unit(iaCode_,levels);
}


//Star operator overloading, providing a random crossing of the current
//unit and the one provided in parameter.
Unit Unit::operator*(const Unit& unit)const
{
    std::vector<int> maxLevels(7);
    for(int i = 0; i < 7; ++i) {
        maxLevels[i] = std::max(capacities_[i]->getLevel(), unit.getCapacity(i)->getLevel());
    }
    int gA = this->getLevel();
    int gB = unit.getLevel();
    int global = gA;
    if(gA!=gB)
        global = std::min(gA, gB)+std::rand()%std::abs(gA-gB);
    std::vector<int> levels(7,0);
    while(global) {
        int index = std::rand()%7;
        if(levels[index]<maxLevels[index]) {
            levels[index]++;
            global--;
        }
    }
    if(std::rand()%2)
        return Unit(iaCode_,levels);
    return Unit(unit.getIACode(),levels);
}


//Save the current unit in the given output stream.
void Unit::save(std::ostream& out)const
{
    std::for_each(capacities_.begin(), capacities_.end(),  [&out](const std::unique_ptr<Capacity>& c) {
        out<<c->getLevel()<<" ";
    });
    out<<iaCode_<<std::endl;
}


//Load a unit from the input stream
Unit Unit::load(std::istream& in)
{
    in.exceptions(std::istream::failbit | std::istream::eofbit);
    std::vector<int> levels(7);
    std::string iacode;
    std::for_each(levels.begin(), levels.end(), [&in](int& level) {
        in >> level;
    });
    in >> iacode;
    return Unit(iacode, levels);
}
