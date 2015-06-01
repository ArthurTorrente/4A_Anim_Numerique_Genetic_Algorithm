#ifndef _STICKY_H_
#define _STICKY_H_

#include "cinder\TriMesh.h"
#include "cinder/gl/gl.h"

#include "cinder/Color.h"

class Sticky
{
public:
	Sticky();
	Sticky(float x, float y);
	Sticky(const cinder::Color& color);
	Sticky(float x, float y, const cinder::Color& color);
    ~Sticky();

	void draw();
protected:
	void CreateMesh();

    float m_x;
    float m_y;
	cinder::TriMesh m_StickyMesh;
	cinder::Color m_Color;
};

#endif //_STICKY_H_