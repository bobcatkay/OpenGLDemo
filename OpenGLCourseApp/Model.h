#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <glm\glm.hpp>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void LoadModelInstance(const std::string& fileName, glm::mat4* modelMatrices, GLuint amount);
	void RenderModel();
	void ClearModel();
	void Model::RenderInstance();
	glm::vec3 initPosition;
	glm::vec3 initScale;
	glm::mat4* modelMatrices;
	GLuint instanceAmount;

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	~Model();

private:

	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);

	std::vector<unsigned int> meshToTex;
};

