#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch,GLfloat startMoveSpeed,GLfloat startTurnSpeed);
	~Camera();

	void KeyControl(bool* keys,GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);
	glm::vec3 GetCameraPosition();
	glm::mat4 CalculoateViewMatirx();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

