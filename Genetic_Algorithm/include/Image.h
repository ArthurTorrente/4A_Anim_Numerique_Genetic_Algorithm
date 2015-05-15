#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

/**
 *
 */
struct DrawableImage
{
    cinder::Surface m_image;
    cinder::gl::Texture m_texture;
};

#endif //_IMAGE_H_