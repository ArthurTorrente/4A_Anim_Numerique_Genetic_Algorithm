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

    float getX() const;
    float getY() const;
    const cinder::ColorA& getColor() const;
    const cinder::TriMesh getMesh() const;
    
    cinder::ColorA& getColor();
    cinder::TriMesh& getMesh();
    float& getX();
    float& getY();
    
    void ChangeColor(const cinder::ColorA& color);
	void draw() const;
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