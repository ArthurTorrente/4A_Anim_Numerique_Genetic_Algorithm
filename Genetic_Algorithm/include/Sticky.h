#ifndef _STICKY_H_
#define _STICKY_H_

#include "cinder\TriMesh.h"
#include "cinder/gl/gl.h"
#include "tools.h"

class Sticky
{
public:
	Sticky();
	Sticky(int x, int y);
	Sticky(rgbaColor color);
	Sticky(int x, int y, rgbaColor color);
	void draw();
	~Sticky();
protected:
	void CreateMesh();

	cinder::TriMesh m_StickyMesh;
	int m_x, m_y;
	rgbaColor m_Color;
};

#endif //_STICKY_H_