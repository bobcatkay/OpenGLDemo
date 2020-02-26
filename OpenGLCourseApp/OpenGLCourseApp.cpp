// OpenGLCourseApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 1920, HEIGHT = 1080;
const float TO_RADIANS = 3.14159265f / 180.0f;
GLuint VBO, VAO, IBO, sShader,uniformModel,uniformProjection;

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

void CreateTriangle()
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
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType) 
{
	GLuint shader = glCreateShader(shaderType);
	const GLchar* code[1];
	code[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the " <<shaderType << ": "<< eLog << std::endl;
		return;
	}
	
	glAttachShader(program, shader);
}

void CompileShaders()
{
	sShader = glCreateProgram();
	
	if (!sShader) {
		std::cout << "Create shader program failed!\n";
		return;
	}

	AddShader(sShader, vShader, GL_VERTEX_SHADER);
	AddShader(sShader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(sShader);
	glGetProgramiv(sShader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(sShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program: " << eLog << std::endl;
		return;
	}

	glValidateProgram(sShader);
	glGetProgramiv(sShader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(sShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error validate program: " << eLog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(sShader, "model");
	uniformProjection = glGetUniformLocation(sShader, "projection");
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

	CreateTriangle();
	CompileShaders();

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

		glUseProgram(sShader);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		model = glm::rotate(model, curAngle * TO_RADIANS, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

}
