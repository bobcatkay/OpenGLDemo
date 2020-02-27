#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
class Shader
{
public:
	Shader();
	void CreateFromString(const char* vetexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	void UseShader();
	void ClearShader();
	~Shader();
private:
	GLuint shaderID, uniformProjection, uniformModel,uniformView;
	void ComplieShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
};

