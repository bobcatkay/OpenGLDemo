#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <ctime>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include "Model.h"

#include "Skybox.h"

unsigned int WindowWidth = 1920;
unsigned int WindowHeight = 1080;

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;
std::vector<Mesh*> meshList;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Material shinyMaterial;
Material dullMaterial;

Model jupitor;
Model rock;
std::vector<Model> ringModels;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat rotateAngle = 0.0f;
GLfloat rockRotateAngle = 0.0f;

GLuint rockAmount = 500;
glm::mat4* modelMatrices;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene(GLfloat deltaTime)
{
	glm::mat4 model;

	rotateAngle += 0.1f*deltaTime;
	if (rotateAngle > 360.0f)
	{
		rotateAngle = 0.1f;
	}

	rockRotateAngle += 10.5f * deltaTime;
	if (rockRotateAngle > 360.0f)
	{
		rockRotateAngle = 0.1f;
	}

	/*for (size_t i = 0; i < ringModels.size(); i++) {
		model = glm::mat4();
		model = glm::rotate(model, rockRotateAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, ringModels[i].initPosition);
		model = glm::scale(model, ringModels[i].initScale);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ringModels[i].RenderModel();
	}*/


	//glUniform1i(uniformIntanceMode, 0);
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	for (int i = 0; i < rockAmount; i++) {
		glm::mat4 model = modelMatrices[i];
		model = glm::rotate(model, rockRotateAngle* toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rock.RenderModel();
	}

	
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotateAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
	//glUniform1i(uniformIntanceMode, 0);
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	jupitor.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();

	RenderScene(deltaTime);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene(deltaTime);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GLfloat deltaTime)
{
	glViewport(0, 0, WindowWidth, WindowHeight);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	//shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	//shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.getShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0].Validate();

	RenderScene(deltaTime);
}

glm::mat4* GenerateRocksModelMatrices(GLuint amount) {
	
	
	modelMatrices = new glm::mat4[amount];
	GLfloat radius = 180*1000.0f;
	GLfloat offset = 15000.0f;
	srand(glfwGetTime()); // initialize random seed
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model=glm::mat4(1.0f);
		// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset )) - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset)) - offset;
		GLfloat y = displacement * 0.4f; // y value has smaller displacement
		displacement = (rand() % (GLint)(2 * offset)) - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		// 2. Scale: Scale between 0.5 and 1.2f

		float scale = (rand() % 6) / 10.0f + 0.7;
		model = glm::scale(model, glm::vec3(scale,scale,scale));
		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		float rx = (1.0f + rand() % 10)/10.0f;
		float ry = (1.0f + rand() % 10)/10.0f;
		float rz = (1.0f + rand() % 10)/10.0f;
		model = glm::rotate(model, rotAngle, glm::vec3(rx, ry, rz));
		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}

	return modelMatrices;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_1) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (key == GLFW_KEY_2) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (key == GLFW_KEY_EQUAL) {
		//glm::mat4* matrioces = GenerateRocksModelMatrices(100);
		rockAmount += 100;
		delete modelMatrices;
		GenerateRocksModelMatrices(rockAmount);
	}
}

int main() 
{
	mainWindow = Window(WindowWidth, WindowHeight); // 1280, 1024 or 1024, WindowHeight
	mainWindow.Initialise();

	CreateShaders();

	//camera = Camera(glm::vec3(900*1000.0f, 50.0f, 8800*1000.0f), glm::vec3(0.0f, 1.0f, 0.0f), 268.0f, -25.0f, 500.0f, 0.5f);
	camera = Camera(glm::vec3(0.0f, 5000.0f, 200*1000.0f), glm::vec3(0.0f, 1.0f, 0.0f), 268.0f, -2.0f, 5000.0f, 0.5f);

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	GenerateRocksModelMatrices(rockAmount);
	rock = Model();
	//rock.LoadModelInstance("Models/rock.obj", modelMatrices, rockAmount);
	rock.LoadModel("Models/rock.fbx");

	//std::cout << "Rocks count:" << models.size() << std::endl;

	jupitor = Model();
	jupitor.LoadModel("Models/jupitor2.fbx");



	mainLight = DirectionalLight(1024, 1024,
								1.0f, 0.9f, 0.9f, 
								0.00f, 0.9f,
								30.0f, -10.0f, -50.0f);

	pointLights[0] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 2.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	//pointLightCount++;
	pointLights[1] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		-4.0f, 3.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	//pointLightCount++;


	spotLights[0] = SpotLight(1024, 1024,
		0.01f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	//spotLightCount++;
	spotLights[1] = SpotLight(1024, 1024,
		0.01f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	//spotLightCount++;



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/space.jpg");
	skyboxFaces.push_back("Textures/Skybox/space.jpg");
	skyboxFaces.push_back("Textures/Skybox/space.jpg");
	skyboxFaces.push_back("Textures/Skybox/space.jpg");
	skyboxFaces.push_back("Textures/Skybox/space.jpg");
	skyboxFaces.push_back("Textures/Skybox/space.jpg");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;

	glm::mat4 projection = glm::perspective(glm::radians(50.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 600000.0f);
	glm::mat4 view = camera.calculateViewMatrix();

	glfwSetKeyCallback(mainWindow.mainWindow, KeyCallback);
	// Loop until window closed
	GLfloat drawBegin;
	GLfloat lastUpdateFPS = 0;
	while (!mainWindow.getShouldClose())
	{
		drawBegin = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = drawBegin - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = drawBegin;
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		/*if (mainWindow.getsKeys()[GLFW_KEY_L])
		{
			spotLights[0].Toggle();
			mainWindow.getsKeys()[GLFW_KEY_L] = false;
		}*/

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}

		RenderPass(camera.calculateViewMatrix(), projection, deltaTime);

		
		mainWindow.swapBuffers();
		// Get + Handle User Input
		glfwPollEvents();

		
		float fps = 1.0f/(glfwGetTime() - drawBegin);
		if (glfwGetTime() - lastUpdateFPS > 0.2) {
			std::cout << "FPS:" << fps << std::endl;
			lastUpdateFPS = glfwGetTime();
		}
		/*if (fps > 60) {
			models.push_back(bowlingPing);
		}
		else if (fps < 30 && models.size()>10) {
			models.pop_back();
		}*/
	}

	delete[] modelMatrices;

	return 0;
}