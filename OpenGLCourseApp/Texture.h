#pragma once

#include <GL\glew.h>

#include "CommonValues.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();
	GLuint textureID;

	~Texture();

private:
	int width, height, bitDepth;

	const char* fileLocation;
};

