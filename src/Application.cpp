/*
 * Application.cpp
 *
 *  Created on: Jul 27, 2014
 *      Author: michal
 */

#include "Application.hpp"

void Application::onInit() {
	theProgram = InitializeProgram();

	InitializeVertexBuffer();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void Application::onDisplay() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(theProgram->getHandle());

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	glutSwapBuffers();
}

void Application::onReshape(int width, int height) {
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

void Application::onKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

shared_ptr<shader::Program> Application::InitializeProgram() {

	vector<shared_ptr<shader::Shader>> shaderList = {
		shader::VertexShader::fromFile("res/shaders/simple.vert"),
		shader::FragmentShader::fromFile("res/shaders/simple.frag")
	};

	return make_shared<shader::Program>(shaderList);
}

void Application::InitializeVertexBuffer() {
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
