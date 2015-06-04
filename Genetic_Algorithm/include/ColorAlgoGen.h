#ifndef _COLORALGOGEN_H_
#define _COLORALGOGEN_H_

#include "IAlgoGen.h"

class ColorAlgoGen : public IAlgoGen
{
public:
    ColorAlgoGen(const cinder::Surface& = cinder::Surface());

    virtual std::vector<Stixel> operator()(const std::vector<Stixel>&) const;

    unsigned int& getNumberOfGenerateChild();
    unsigned int getNumberOfGenerateChild() const;

    void setNumberOfGenerateChild(unsigned int);

protected:
    unsigned int m_numberOfChild;
    unsigned int getFitness(const Sticky&, const cinder::ColorA&) const;
};

#endif //_COLORALGOGEN_H_