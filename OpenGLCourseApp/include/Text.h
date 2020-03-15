#pragma once

#include <iostream>
#include <map>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Shader.h"

class Text
{
public:
	Text();
	void Init(GLuint windowWidth, GLuint windowHeight, std::string fontPath);
	void RenderText(Shader& shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	~Text();
private:
	struct Character {
		GLuint TextureID;   // ID handle of the glyph texture
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
		GLuint Advance;    // Horizontal offset to advance to next glyph
	};

	GLuint wWidth, wHeight;

	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
	std::string fontPath;
	unsigned int fontSize = 16;


};

