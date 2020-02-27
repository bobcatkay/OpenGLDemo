#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Window
{
public:
	Window();
	~Window();
	
	Window(GLint windowWidth, GLint windowHeight);
	int Initialize();
	GLfloat getBufferWidth() { return bufferHeight; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

