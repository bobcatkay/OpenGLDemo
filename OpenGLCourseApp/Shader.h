#pragma once
#include <iostream>
#include <fstream>

#include <GL/glew.h>
class Shader
{
public:
	Shader();
	void CreateFromString(const char* vetexCode, const char* fragmentCode);
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	void UseShader();
	void ClearShader();
	~Shader();
private:
	GLuint shaderID, uniformProjection, uniformModel;
	void ComplieShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
};

