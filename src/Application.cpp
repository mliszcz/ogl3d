/*
 * Application.cpp
 *
 *  Created on: Jul 27, 2014
 *      Author: michal
 */

#include "Application.hpp"

void Application::onInit() {

	program = make_shared<shader::Program>(initializer_list<shared_ptr<shader::Shader>> {
			shader::VertexShader::fromFile("res/shaders/simple.vert"),
			shader::FragmentShader::fromFile("res/shaders/simple.frag")
	});

	buffer = make_shared<gfx::GenericBuffer>();
	buffer->realloc({
		     0.0f,    0.5f, 0.0f, 1.0f,
		     0.5f, -0.366f, 0.0f, 1.0f,
		    -0.5f, -0.366f, 0.0f, 1.0f,
		     1.0f,    0.0f, 0.0f, 1.0f,
		     0.0f,    1.0f, 0.0f, 1.0f,
		     0.0f,    0.0f, 1.0f, 1.0f,
	});

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
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

void Application::onDisplay() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program->getHandle());

	glBindBuffer(GL_ARRAY_BUFFER, buffer->getHandle());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) 48);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}
