//============================================================================
// Name        : ogl3d.cpp
// Author      : michal
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Common.hpp"
#include "util/Config.hpp"
#include "util/Logger.hpp"

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
	std::cout << "here1!\n";

	GLuint shader = glCreateShader(eShaderType);

	std::cout << "here1!\n";

	const char *strFileData = strShaderFile.c_str();

	std::cout << "here1!\n";

	glShaderSource(shader, 1, &strFileData, NULL);

	std::cout << "try compile!\n";

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "shader failed!\n";
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
	std::cout << "here1!\n";

	GLuint program = glCreateProgram();

	std::cout << "here2!\n";

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	std::cout << "here3!\n";

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::cout << "failed!\n";
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

GLuint theProgram;

const std::string strVertexShader("#version 330\n"
		"layout(location = 0) in vec4 position;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = position;\n"
		"}\n");

const std::string strFragmentShader("#version 330\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
		"   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n");

void InitializeProgram() {
	std::vector<GLuint> shaderList;

	std::cout << "compiling!\n";
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

	std::cout << "done!\n";

	theProgram = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

const float vertexPositions[] = { 0.75f, 0.75f, 0.0f, 1.0f, 0.75f, -0.75f, 0.0f, 1.0f, -0.75f, -0.75f, 0.0f, 1.0f, };

GLuint positionBufferObject;
GLuint vao;

void InitializeVertexBuffer() {
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init() {
	InitializeProgram();
	std::cout << "prog!\n";
	InitializeVertexBuffer();

	std::cout << "InitializeVertexBuffer!\n";

	glGenVertexArrays(1, &vao);

	std::cout << "glGenVertexArrays!\n";

	glBindVertexArray(vao);

	std::cout << "init!\n";
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(theProgram);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	glutSwapBuffers();
}

void reshape(int width, int height) {
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

int main(int argc, char** argv) {

	auto config = util::Config::getInstance("res/ogl3d.config");
	auto logLvl = util::Logger::LoggerLevel::INFO;
	auto logger = util::Logger::getInstance(std::cout, logLvl);

	logger->info("fs: %d\n", config->get("FULLSCREEN"));

	glutInit(&argc, argv);

	int width = config->get("WINDOW_WIDTH");
	int height = config->get("WINDOW_HEIGHT");

	unsigned int displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

	glutInitDisplayMode(displayMode);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_DEBUG);

	std::cout << "one!\n";

	glutInitWindowSize(width, height);
//	glutInitWindowPosition(300, 200);
	glutCreateWindow(argv[0]);

	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	std::cout << "two!\n";

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
