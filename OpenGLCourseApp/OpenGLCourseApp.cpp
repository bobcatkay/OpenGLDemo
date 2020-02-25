// OpenGLCourseApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 1920, HEIGHT = 1080;
GLuint VBO, VAO, sShader;

//Vertex Shader
static const char* vShader = "									\n\
#version 330													\n\
																\n\
layout (location = 0) in vec3 pos;								\n\
																\n\
void main()														\n\
{																\n\
	gl_Position = vec4(0.1 * pos.x, 0.1 * pos.y, pos.z, 1.0);	\n\
}";

//Fragment Shader
static const char* fShader = "									\n\
#version 330													\n\
																\n\
out vec4 colour;												\n\
																\n\
void main()														\n\
{																\n\
	colour = vec4(0.1f, 0.9f, 0.3f, 1.0);						\n\
}";

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

	//Setup Viewport size
	glViewport(0, 0, bufWidth, bufHeight);

	CreateTriangle();
	CompileShaders();

	//loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Get + handle user input events
		glfwPollEvents();

		//Clear window
		glClearColor(0.0f, 0.0f, 0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(sShader);

		glBindVertexArray(VAO);
		for (int i = 0; i < 1000000; i++) {

			glDrawArrays(GL_TRIANGLES,0,3);
		}
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
