// OpenGLCourseApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"

#include <cmath>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION

Window mainWindow;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

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
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f,1.0f,0.0f),-90.0f,0.0f,4.0f,0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/ mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Get + handle user input events
		glfwPollEvents();

		camera.KeyControl(mainWindow.getKeys(),deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		/*if (direction) {
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
		}*/

		//Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		model = glm::rotate(model, curAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-triOffset, 1.0f, -2.5f));
		model = glm::rotate(model, curAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculoateViewMatirx()));

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

}
