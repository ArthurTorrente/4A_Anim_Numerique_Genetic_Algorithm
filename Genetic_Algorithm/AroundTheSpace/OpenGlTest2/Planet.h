#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <noise/noise.h>

#include "Vector3.h"
#include "Vector2.h"
#include "VertexDataP.h"
//#include "noiseutils.h"
#include "materials.h"


class Planet
{
public:
	Planet();
	~Planet();

	std::vector<Vector3> vertices;
	std::vector<Vector3> normales;
	std::vector<Vector2> uvs;

	std::vector<VertexDataPNT> Vnu;
	std::vector<GLuint> triangles;
	materials* mat;

private:
	void generatePlanet();
	//utils::NoiseMap generateHeightMap();
	//void generateTexture(utils::NoiseMap heightMap);
};

