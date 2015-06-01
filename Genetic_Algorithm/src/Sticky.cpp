#include "Sticky.h"

Sticky::Sticky()
{
	//CreateMesh();
}

Sticky::Sticky(float x, float y)
{
	this->m_x = x;
	this->m_y = y;
	//CreateMesh();
}
Sticky::Sticky(const cinder::ColorA& color)
{
	this->m_Color = color;
	//CreateMesh();
}

Sticky::Sticky(float x, float y, const cinder::ColorA& color)
{
	this->m_x = x;
	this->m_y = y;
	this->m_Color = color;
	//CreateMesh();
}

Sticky::~Sticky()
{}

float Sticky::getX() const
{
    return m_x;
}

float Sticky::getY() const
{
    return m_y;
}

const cinder::ColorA& Sticky::getColor() const
{
    return m_Color;
}

const cinder::TriMesh Sticky::getMesh() const
{
    return m_StickyMesh;
}

cinder::ColorA& Sticky::getColor()
{
    return m_Color;
}

cinder::TriMesh& Sticky::getMesh()
{
    return m_StickyMesh;
}

float& Sticky::getX()
{
    return m_x;
}

float& Sticky::getY()
{
    return m_y;
}

void Sticky::CreateMesh()
{
	// Create the points of our cube
	cinder::Vec3f v0(-100, -100, -100);
	cinder::Vec3f v1(100, -100, -100);
	cinder::Vec3f v2(100, 100, -100);
	cinder::Vec3f v3(-100, 100, -100);
	cinder::Vec3f v4(-100, -100, 100);
	cinder::Vec3f v5(100, -100, 100);
	cinder::Vec3f v6(100, 100, 100);
	cinder::Vec3f v7(-100, 100, 100);

	cinder::Vec3f faces[6][4] = { /* Vertices for the 6 faces of a cube. */
		{ v0, v1, v2, v3 }, { v3, v2, v6, v7 }, { v7, v6, v5, v4 },
		{ v4, v5, v1, v0 }, { v5, v6, v2, v1 }, { v7, v4, v0, v3 } };

	for (int i = 0; i < 6; i++)
	{

		this->m_StickyMesh.appendVertex(faces[i][0]);
		this->m_StickyMesh.appendVertex(faces[i][1]);
		this->m_StickyMesh.appendVertex(faces[i][2]);
		this->m_StickyMesh.appendVertex(faces[i][3]);
		int numberVertices = this->m_StickyMesh.getNumVertices();

		this->m_StickyMesh.appendTriangle(numberVertices - 4, numberVertices - 3, numberVertices - 2);
		this->m_StickyMesh.appendTriangle(numberVertices - 4, numberVertices - 2, numberVertices - 1);
	}
}

void Sticky::draw() const
{
	cinder::Vec3f center(m_x, m_y, 1);
	cinder::Vec3f size(1, 1, 1);
    cinder::gl::color(m_Color);
	cinder::gl::drawColorCube(center, size);
}