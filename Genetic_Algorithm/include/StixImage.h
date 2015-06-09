#ifndef _STIXIMAGE_H_
#define _STIXIMAGE_H_

#include <vector>
#include "cinder/Color.h"

#include "Stixel.h"
/**
 * Represente une image
 * Composé de stixel
 */
class StixImage
{
public:
    StixImage();
    ~StixImage();
private:
    std::vector<Stixel> m_data;
};

#endif //_STIXIMAGE_H_