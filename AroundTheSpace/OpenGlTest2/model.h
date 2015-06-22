#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VertexBuffer.h"

GLint TextureFromFile(const char* path, string directory);

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Model
{
public:
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model(GLchar* path, ShaderInterface* shader)
	{
		this->loadModel(path, shader);
	}

	// Draws the model, and thus all its meshes
	/*void Draw(Shader shader)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader);
	}*/
	vector<VertexBuffer*> meshes;
private:
	/*  Model Data  */
	
	string directory;
	vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	/*  Functions   */
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string path, ShaderInterface* shader)
	{
		// Read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// Retrieve the directory path of the filepath
		this->directory = path.substr(0, path.find_last_of('/'));

		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene, shader);
	}

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene, ShaderInterface* shader)
	{
		// Process each mesh located at the current node
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene, shader));
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene, shader);
		}

	}

	VertexBuffer* processMesh(aiMesh* mesh, const aiScene* scene, ShaderInterface* shader)
	{
		std::vector<VertexDataPNT> Vnu;
		// Data to fill
		
		vector<GLuint> indices;
		vector<Texture> textures;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			VertexDataPNT pnt;
			//Vertex vertex;
			Vector3 vertex = makeVector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z); // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			pnt.positionCoordinates = vertex;
			// Normals
			Vector3 normal = makeVector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z); // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

			pnt.normalCoordinates = normal;
			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				Vector2 tex = makeVector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				pnt.textureCoordinates = tex;
			}
			else
				pnt.textureCoordinates = makeVector2(0.0f,0.0f);
			Vnu.push_back(pnt);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		vector<materials*>*_materials = new std::vector<materials*>();

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN
			
			
			// 1. Diffuse maps
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			
			// 2. Specular maps
			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			//_materials->push_back(new materials(diffuseMaps.at(0).id, 32.0f));
			
		}
		//_materials->push_back(new materials("Rock-Texture-Surface.jpg", "Rock-Texture-Surface.jpg", 32.0f));
		VertexBuffer* vertexBuff = new VertexBuffer(Vnu, Vnu.size() * sizeof(Vnu) * 2, GL_TRIANGLES, indices.size(), sizeof(VertexDataPNT), shader, NULL, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), indices, true, 10000);
		//vertexBuff->set_materials(_materials);
		// Return a mesh object created from the extracted mesh data
		return vertexBuff;
	}

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
};




GLint TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}