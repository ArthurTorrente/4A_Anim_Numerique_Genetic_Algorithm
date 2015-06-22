#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <vector>
#include <noise/noise.h>

#include "Vector3.h"
#include "Vector2.h"
#include "VertexDataP.h"
#include "noiseutils.h""


class Planet
{
public:
	Planet();
	~Planet();

	std::vector<Vector3> vertices;
	std::vector<Vector3> normales;
	std::vector<Vector2> uvs;
<<<<<<< HEAD
	std::vector<VertexDataPNT> Vnu;
	std::vector<GLuint> triangles;
=======
	std::vector<int> triangles;
	unsigned char texture[393216]; // texture de 512 par 256
>>>>>>> origin/fred

private:
	void generatePlanet();
	utils::NoiseMap generateHeightMap();
	void generateTexture(utils::NoiseMap heightMap);
};

