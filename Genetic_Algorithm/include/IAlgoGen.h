#ifndef _ALGOGEN_H_
#define _ALGOGEN_H_

#include <vector>

#include "cinder/gl/Texture.h"

#include "Stixel.h"

class IAlgoGen
{
public:
   virtual std::vector<Stixel> operator()(const std::vector<Stixel>&) const = 0;
};

#endif //_ALGOGEN_H_