#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateMeshInstance(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices, 
		glm::mat4* modelMatrices,GLuint instanceAmount);
	void RenderMesh();
	void RenderInstance();
	void ClearMesh();

	~Mesh();

private:
	GLsizei indexCount;
	GLuint VBO, IBO, VAO;
	glm::mat4* modelMatrices;
	GLuint instanceAmount;
};

