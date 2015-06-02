#include "Sticky.h"

#include "cinder/Rand.h"

#include <bitset>

#define BITCOUNTFLOAT 32

Sticky::Sticky()
{
	this->m_x = 0.f;
	this->m_y = 0.f;
	this->m_Color = cinder::ColorA::black();
	CreateMesh();
}

Sticky::Sticky(const Sticky& s)
    : m_width(s.m_width),
    m_height(s.m_height),
    m_x(s.m_x),
    m_y(s.m_y),
    m_StickyMesh(s.m_StickyMesh),
    m_Color(s.m_Color)
{}

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
    this->m_StickyMesh.clear();

	// Create the points of our cube
	cinder::Vec3f v0(this->m_x - this->m_width / 2, this->m_y - this->m_height / 2, 1);
    cinder::Vec3f v1(this->m_x + this->m_width / 2, this->m_y - this->m_height / 2, 1);
    cinder::Vec3f v2(this->m_x + this->m_width / 2, this->m_y + this->m_height / 2, 1);
    cinder::Vec3f v3(this->m_x - this->m_width / 2, this->m_y + this->m_height / 2, 1);
    cinder::Vec3f v4(this->m_x - this->m_width / 2, this->m_y + this->m_height / 2, -1);
    cinder::Vec3f v5(this->m_x + this->m_width / 2, this->m_y + this->m_height / 2, -1);
    cinder::Vec3f v6(this->m_x + this->m_width / 2, this->m_y - this->m_height / 2, -1);
    cinder::Vec3f v7(this->m_x - this->m_width / 2, this->m_y - this->m_height / 2, -1);

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

void Sticky::ChangeColor(const cinder::ColorA& color)
{
    for (auto& currentColor : this->m_StickyMesh.getColorsRGBA())
    {
        currentColor = color;
    }
}

void Sticky::draw() const
{
	cinder::gl::draw(this->m_StickyMesh);
}


void Sticky::updateSize(float wRatio, float hRatio)
{
    cinder::Vec3f newVertexPoint;
    int i = 0;
    this->m_height *= hRatio;
    this->m_width *= wRatio;
    this->m_x *= wRatio;
    this->m_y *= hRatio;
    for (auto& vertex : this->m_StickyMesh.getVertices())
    {
        //x
        if (i == 1 || i == 2 || i == 5 || i == 6)
            vertex.x = this->m_x + (this->m_width) / 2;
        else
            vertex.x = this->m_x  - (this->m_width ) / 2;
        //y
        if (i < 2 || i > 5)
            vertex.y = this->m_y  - this->m_height /2;
        else
            vertex.y = this->m_y  + this->m_height / 2;
        //
        if (i < 4)
            vertex.z = 1;
        else
            vertex.z = -1;
        i++;
    }
}

static float genRadomFloat(float a, float b)
{
    static unsigned int cpt = 0;

    cinder::Rand randomizer(cpt++);
    size_t bitCount = BITCOUNTFLOAT;

    unsigned int result = static_cast<unsigned int>(a);
    unsigned int bitIterator = 1;

    for (unsigned int i = 0; i < bitCount; ++i)
    {
        //Inverse le bit bitIterator de result
        if (randomizer.nextBool())
        {
            result = (result & bitIterator) ? result - bitIterator : result + bitIterator;
        }

        bitIterator = bitIterator << 1;
    }

    return static_cast<float>(result);
}


Sticky Sticky::operator*(const Sticky& s) const
{
    Sticky newSticky(*this);
    
    float rgb[3] {
        genRadomFloat(m_Color.r, s.m_Color.r),
        genRadomFloat(m_Color.g, s.m_Color.g),
        genRadomFloat(m_Color.b, s.m_Color.b)
    };

    newSticky.ChangeColor(cinder::ColorA(rgb[0], rgb[1], rgb[2], m_Color.a));

    return newSticky;
}

Sticky Sticky::mutate() const
{
    Sticky newSticky(*this);



    return newSticky;
}

Sticky Sticky::random()
{
    Sticky newSticky(*this);

    return newSticky;
}


#if 0
void Sticky::updateSize(float wRatio, float hRatio)
{
    this->m_height *= hRatio;
    this->m_width *= wRatio;
    this->m_x *= wRatio;
    this->m_y *= hRatio;
    this->CreateMesh();
}
#endif