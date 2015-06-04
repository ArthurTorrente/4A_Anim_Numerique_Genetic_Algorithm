#ifndef _COLORALGOGEN_H_
#define _COLORALGOGEN_H_

#include "IAlgoGen.h"

class ColorAlgoGen : public IAlgoGen
{
public:
    ColorAlgoGen();

    virtual std::vector<Stixel> operator()(const std::vector<Stixel>&) const;

    unsigned int& getNumberOfGenerateChild();
    unsigned int getNumberOfGenerateChild() const;

    unsigned char& getMutationRatio();
    unsigned char& getCombinaisonRatio();
    unsigned char& getRandomRatio();

    void setNumberOfGenerateChild(unsigned int);

protected:
    unsigned int m_numberOfChild;
    
    unsigned char m_mutationRatio;
    unsigned char m_combinaisonRatio;
    unsigned char m_randomRatio;

    unsigned int getFitness(const Sticky&, const cinder::ColorA&) const;
};

#endif //_COLORALGOGEN_H_