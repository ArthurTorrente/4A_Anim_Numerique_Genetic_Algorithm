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

    unsigned char& getInterval(IntervalType);
    unsigned char getInterval(IntervalType) const;

    float getPercent(IntervalType) const;

    unsigned int getFitness(const Sticky&, const cinder::ColorA&) const;

protected:
    
    std::vector<Interval> m_interval;
};

#endif //_COLORALGOGEN_H_