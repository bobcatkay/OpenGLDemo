// OpenGLCourseApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Shader.h"

const GLint WIDTH = 1920, HEIGHT = 1080;
const float TO_RADIANS = 3.14159265f / 180.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.6f;
float triIncrement = 0.005f;
float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//Vertex Shader
static const char* vShader = "									\n\
#version 330													\n\
																\n\
layout (location = 0) in vec3 pos;								\n\
out vec4 vCol;													\n\
uniform mat4 model;												\n\
uniform mat4 projection;										\n\
																\n\
void main()														\n\
{																\n\
	gl_Position = projection * model * vec4(pos, 1.0);						\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);					\n\
}";

//Fragment Shader
static const char* fShader = "									\n\
#version 330													\n\
																\n\
in vec4 vCol;													\n\
out vec4 colour;												\n\
																\n\
void main()														\n\
{																\n\
	colour = vCol;												\n\
}";

void CreateObjects()
{
	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};
	GLfloat vertices[] = {
		-1.0f,-1.0f,0.0f,
		0.0f,-1.0f,1.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};
	
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromString(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	//Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW initialisation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Setup GLFW window properties
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//Allow msaa
	glfwWindowHint(GLFW_SAMPLES, 64);


	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow) {
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	//Get buffer size information
	int bufWidth, bufHeight;
	glfwGetFramebufferSize(mainWindow, &bufWidth, &bufHeight);

	//Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);
	
	//Allow mordern extension features
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK) 
	{
		std::cout << "GLEW initialization failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	//Setup Viewport size
	glViewport(0, 0, bufWidth, bufHeight);

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufWidth / (GLfloat)bufHeight, 0.1f, 100.0f);

	//loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Get + handle user input events
		glfwPollEvents();

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		curAngle += 1.0f;
		if (curAngle >= 360) {
			curAngle -= 360;
		}

		if (sizeDirection) {
			curSize += 0.005f;
		}
		else {
			curSize -= 0.005f;
		}

		if (curSize >= maxSize || curSize <= minSize) {
			sizeDirection = !sizeDirection;
		}

		//Clear window
		glClearColor(0.0f, 0.0f, 0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		model = glm::rotate(model, curAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-triOffset,1.0f, -2.5f));
		model = glm::rotate(model, curAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

}
