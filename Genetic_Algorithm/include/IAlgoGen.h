#ifndef _ALGOGEN_H_
#define _ALGOGEN_H_

#include <vector>

#include "cinder/gl/Texture.h"

#include "Sticky.h"

class IAlgoGen
{
public:
    IAlgoGen(const cinder::Surface&);

    cinder::Surface getComputeImage() const;
    void setComputeImage(const cinder::Surface&);

    virtual std::vector<Sticky> operator()(const std::vector<Sticky>&) const = 0;

protected:
    cinder::Surface m_computeImage;
};

#endif //_ALGOGEN_H_