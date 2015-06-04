#ifndef _COLORALGOGEN_H_
#define _COLORALGOGEN_H_

#include "IAlgoGen.h"

#include <map>

class ColorAlgoGen : public IAlgoGen
{
public:
    enum IntervalType
    {
        MUTATE,
        COMBINAISON,
        RANDOM,
        COPY
    };

    struct Interval
    {
        IntervalType type;
        unsigned char percent;

        Interval(IntervalType _type, unsigned char _percent)
            : type(_type),
            percent(_percent)
        {}
    };

    ColorAlgoGen();

    virtual std::vector<Stixel> operator()(const std::vector<Stixel>&) const;

    unsigned int& getNumberOfGenerateChild();
    unsigned int getNumberOfGenerateChild() const;

    unsigned char& getInterval(IntervalType);
    unsigned char getInterval(IntervalType) const;

    void setNumberOfGenerateChild(unsigned int);

protected:
    unsigned int m_numberOfChild;
    
    std::vector<Interval> m_interval;

    void mutatePopulation(std::vector<Stixel>::const_iterator start, std::vector<Stixel>::const_iterator end, std::vector<Stixel>& nextPopulation) const;
    void combinePopulation(std::vector<Stixel>::const_iterator start, std::vector<Stixel>::const_iterator end, const std::vector<Stixel>& combinePop, std::vector<Stixel>& nextPopulation) const;
    void radomPopulation(std::vector<Stixel>::const_iterator start, std::vector<Stixel>::const_iterator end, std::vector<Stixel>& nextPopulation) const;
    void copyPopulation(std::vector<Stixel>::const_iterator start, std::vector<Stixel>::const_iterator end, std::vector<Stixel>& nextPopulation) const;

    unsigned int getFitness(const Sticky&, const cinder::ColorA&) const;
};

#endif //_COLORALGOGEN_H_