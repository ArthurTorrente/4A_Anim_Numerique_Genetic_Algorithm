#ifndef _COLORALGOGEN_H_
#define _COLORALGOGEN_H_

#include "IAlgoGen.h"

class ColorAlgoGen : public IAlgoGen
{
public:
    ColorAlgoGen(const cinder::Surface& = cinder::Surface());

    virtual std::vector<Sticky> operator()(const std::vector<Sticky>&) const;

protected:
    unsigned int getFitness(const Sticky&, const cinder::ColorA&);
};

#endif //_COLORALGOGEN_H_