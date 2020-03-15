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

	GLuint VBO, IBO, VAO, VBO2;
	GLsizei indexCount;
	~Mesh();

private:
	glm::mat4* modelMatrices;
	GLuint instanceAmount;
};

