#ifndef _ALGOGEN_H_
#define _ALGOGEN_H_

#include <vector>

#include "cinder/gl/Texture.h"

#include "Sticky.h"

class AlgoGen
{
public:
    AlgoGen(const cinder::Surface&);

    cinder::Surface getComputeImage() const;
    void setComputeImage(const cinder::Surface&);

    std::vector<Sticky> operator()(const std::vector<Sticky>&);

protected:
    cinder::Surface m_computeImage;
};

#endif //_ALGOGEN_H_