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
	Sticky(const cinder::ColorA& color);
	Sticky(float width, float height, float x, float y, const cinder::ColorA& color);
    ~Sticky();

	void draw();
protected:
	void CreateMesh();

	float m_width;
	float m_height;

    float m_x;
    float m_y;
	cinder::TriMesh m_StickyMesh;
	cinder::ColorA m_Color;
};

#endif //_STICKY_H_