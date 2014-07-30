/*
 * Application.hpp
 *
 *  Created on: Jul 30, 2014
 *      Author: michal
 */

#ifndef APPLICATION_HPP_
#define APPLICATION_HPP_

#include "Common.hpp"
#include "ApplicationBase.hpp"

#include "util/Singleton.hpp"
#include "util/Config.hpp"
#include "util/Logger.hpp"

#include "shader/Shader.hpp"
#include "shader/VertexShader.hpp"
#include "shader/FragmentShader.hpp"
#include "shader/Program.hpp"

#include "gfx/GenericBuffer.hpp"

class Application : public ApplicationBase, public util::Singleton<Application> {
	friend class util::Singleton<Application>;

private:
	shared_ptr<shader::Program> program = nullptr;
	shared_ptr<gfx::GenericBuffer> buffer = nullptr;

	GLuint vao = 0;

	int offsetLocation = 0;

private:

	Application(int argc, char** argv)
		: ApplicationBase(argc, argv) { }

public:

	virtual ~Application() { }

	virtual void onInit() {

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

		offsetLocation = glGetUniformLocation(*program, "offset");
	}

	virtual void onReshape(int width, int height) {
		glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	}

	void onKeyboard(unsigned char key, int x, int y) {
		switch (key) {
		case 27:
			glutLeaveMainLoop();
			return;
		}
	}

	virtual void onDisplay() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float fXOffset = 0.0f, fYOffset = 0.0f;
//		ComputePositionOffsets(fXOffset, fYOffset);

		glUseProgram(*program);

		glUniform2f(offsetLocation, fXOffset, fYOffset);

		glBindBuffer(GL_ARRAY_BUFFER, *buffer);
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

};

#endif /* APPLICATION_HPP_ */
