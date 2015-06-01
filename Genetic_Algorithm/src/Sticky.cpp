#include "Sticky.h"

Sticky::Sticky()
{
	this->m_x = 0.f;
	this->m_y = 0.f;
	this->m_Color = cinder::ColorA::black();
	CreateMesh();
}

Sticky::Sticky(float x, float y)
{
	this->m_x = x;
	this->m_y = y;
	this->m_Color = cinder::ColorA::black();
	CreateMesh();
}
Sticky::Sticky(const cinder::ColorA& color)
{
	this->m_Color = color;
	CreateMesh();
}

Sticky::Sticky(float width, float height,float x, float y, const cinder::ColorA& color)
{
	this->m_width = width;
	this->m_height = height;
	this->m_x = x;
	this->m_y = y;
	this->m_Color = color;
	CreateMesh();
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
	cinder::Vec3f v0(this->m_x - this->m_width / 2, this->m_y - this->m_width / 2, 1 + this->m_width / 2);
	cinder::Vec3f v1(this->m_x + this->m_width / 2, this->m_y - this->m_width / 2, 1 + this->m_width / 2);
	cinder::Vec3f v2(this->m_x + this->m_width / 2, this->m_y + this->m_width / 2, 1 + this->m_width / 2);
	cinder::Vec3f v3(this->m_x - this->m_width / 2, this->m_y + this->m_width / 2, 1 + this->m_width / 2);
	cinder::Vec3f v4(this->m_x - this->m_width / 2, this->m_y + this->m_width / 2, 1 - this->m_width / 2);
	cinder::Vec3f v5(this->m_x + this->m_width / 2, this->m_y + this->m_width / 2, 1 - this->m_width / 2);
	cinder::Vec3f v6(this->m_x + this->m_width / 2, this->m_y - this->m_width / 2, 1 - this->m_width / 2);
	cinder::Vec3f v7(this->m_x - this->m_width / 2, this->m_y - this->m_width / 2, 1 - this->m_width / 2);

	/*cinder::Vec3f faces[6][4] = { /* Vertices for the 6 faces of a cube. 
		{ v0, v1, v2, v3 }, { v3, v2, v6, v7 }, { v7, v6, v5, v4 },
		{ v4, v5, v1, v0 }, { v5, v6, v2, v1 }, { v7, v4, v0, v3 } };*/

	cinder::Vec3f faces[6][4] = {
			{ v0, v1, v2, v3 }, { v7, v6, v5, v4 }, //front and back faces
			{ v4, v5, v2, v3 }, { v7, v6, v1, v0 }, // up and bot faces
			{ v0, v7, v4, v3 }, { v1, v6, v5, v2 } // left right faces
	};

	for (int i = 0; i < 6; i++)
	{

		this->m_StickyMesh.appendVertex(faces[i][0]);
		this->m_StickyMesh.appendColorRgba(this->m_Color);
		this->m_StickyMesh.appendVertex(faces[i][1]);
		this->m_StickyMesh.appendColorRgba(this->m_Color);
		this->m_StickyMesh.appendVertex(faces[i][2]);
		this->m_StickyMesh.appendColorRgba(this->m_Color);
		this->m_StickyMesh.appendVertex(faces[i][3]);
		this->m_StickyMesh.appendColorRgba(this->m_Color);
		int numberVertices = this->m_StickyMesh.getNumVertices();

		this->m_StickyMesh.appendTriangle(numberVertices - 4, numberVertices - 3, numberVertices - 2);
		this->m_StickyMesh.appendTriangle(numberVertices - 4, numberVertices - 2, numberVertices - 1);
	}
}

void Sticky::draw() const
{
	cinder::gl::draw(this->m_StickyMesh);
}