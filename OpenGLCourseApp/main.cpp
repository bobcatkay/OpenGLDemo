#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <cmath>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "Common.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "PointLight.h"
#include "SpotLight.h"

Window mainWindow;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Material shinyMaterial;
Material dullMaterial;


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

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat *vertices,unsigned int verticeCount,unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i +=3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1],vertices[in1+2]-vertices[in0+2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);

		normal = glm::normalize(normal);
		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;
		vertices[in0] += normal.x;vertices[in0 + 1] += normal.y;vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x;vertices[in1 + 1] += normal.y;vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x;vertices[in2 + 1] += normal.y;vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}
void CreateObjects()
{
	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};
	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f,0.0f,0.0f,
		0.0f, -1.0f, 1.0f,		1.0f, 0.0f,		0.0f,0.0f,0.0f,
		1.0f, -1.0f, -0.6f,		2.0f, 0.0f,		0.0f,0.0f,0.0f,
		0.0f, 1.0f, 0.0f,		1.0f, 2.0f,		0.0f,0.0f,0.0f
	};


	unsigned int floorIndices[] = {
		0,2,1,
		1,2,3
	};

	GLfloat floorVertices[] = {
		-10.0f,0.0f,-10.0f,		0.0f,0.0f,		0.0f,-1.0f,0.0f,
		10.0f,0.0f,-10.0f,		10.0f,0.0f,		0.0f,-1.0f,0.0f,
		-10.0f,0.0f,10.0f,		0.0f,10.0f,		0.0f,-1.0f,0.0f,
		10.0f,0.0f,10.0f,		10.0f,10.0f,	0.0f,-1.0f,0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f,1.0f,0.0f),-90.0f,0.0f,5.0f,0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTexture();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3,4);

	mainLight = DirectionalLight(1.0f,1.0f,1.0f,
								0.1f,0.1f,
								0.0f,0.0f,-1.0f);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.0f, 0.1f,
								0.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.0f, 0.1f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);
	//pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
							0.0f, 1.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, -1.0f, 0.0f,
							0.3f, 0.2f, 0.1f,
							20.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
		uniformAmbientIntensity = 0, uniformAmbientColour = 0, 
		uniformDirection = 0,uniformDiffuseIntensity=0,
		uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShiness = 0;
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
		camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		/*if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}*/

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShiness = shaderList[0].GetShininessLocation();
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		//mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour, uniformDiffuseIntensity, uniformDirection);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculoateViewMatirx()));
		glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);


		glm::mat4 model;
		model = glm::translate(model, glm::vec3(triOffset-3.0f, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShiness);
		meshList[0]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-triOffset, 0.0f, -2.5f));
		//model = glm::rotate(model, curAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShiness);
		meshList[1]->RenderMesh();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-triOffset, -2.0f, -2.5f));
		//model = glm::rotate(model, curAngle * TO_RADIANS, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShiness);
		meshList[2]->RenderMesh();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

}
