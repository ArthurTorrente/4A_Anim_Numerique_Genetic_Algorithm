#ifndef _ALGOGEN_H_
#define _ALGOGEN_H_

#include <vector>

#include "cinder/Surface.h"

#include "Stixel.h"

class IAlgoGen
{
public:
    enum IntervalType
    {
        MUTATE,
        CROSSOVER,
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

    struct SurfaceWrapper
    {
        SurfaceWrapper(const cinder::Surface& s, uint64_t fit)
            : fitness(fit),
            image(s)
        {}
        uint64_t fitness;
        cinder::Surface image;
    };

    IAlgoGen();
    virtual ~IAlgoGen();

    unsigned char& getInterval(IntervalType);
    unsigned char getInterval(IntervalType) const;
    float getPercent(IntervalType) const;
    unsigned int& getPopSize();
    unsigned int getPopSize() const;
    const std::vector<SurfaceWrapper>& getPop() const;
    std::vector<SurfaceWrapper>& getPop();

    void setPopSize(unsigned int popSize);

    /* Setup population and anymore*/
    virtual bool setup(unsigned int width, unsigned int height);

    uint64_t getFitness(const cinder::Surface& realImage, const cinder::Surface& popImage) const;

    /* Prend l'image de base en paramètre et retourne une image qui se rapproche de celle de base */
    virtual cinder::Surface operator()(const cinder::Surface&) = 0;

protected:
    std::vector<Interval> m_interval;
    
    /* Nombre de surface qui seront utilisé dans l'algoGen*/
    unsigned int m_popSize;
    
    /* La population de surface de l'algoGen */
    std::vector<SurfaceWrapper> m_population;

    cinder::Surface getRandomSurface(unsigned int width, unsigned int height) const;
};

#endif //_ALGOGEN_H_