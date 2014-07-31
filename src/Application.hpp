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

#include "gfx/type/Matrix.hpp"
#include "gfx/type/Vector.hpp"
#include "gfx/type/Scalar.hpp"

class Application : public ApplicationBase, public util::Singleton<Application> {
	friend class util::Singleton<Application>;

private:

	shared_ptr<shader::Program> program = nullptr;
	shared_ptr<gfx::GenericBuffer> buffer = nullptr;

	GLuint vao = 0;

	gfx::Matrix<4, 4, float> perspectiveMatrix;
	float fFrustumScale = 1.0f;

private:

	Application(int argc, char** argv)
		: ApplicationBase(argc, argv) { }

public:

	virtual ~Application() { }

	virtual void onInit() {

		auto shaders = {
				shader::VertexShader::fromFile("res/shaders/simple.vert"),
				shader::FragmentShader::fromFile("res/shaders/simple.frag")
		};

		program = make_shared<shader::Program>(shaders);

		float fzNear = 0.5f; float fzFar = 3.0f;

		perspectiveMatrix.at(0,0) = fFrustumScale;
		perspectiveMatrix.at(1,1) = fFrustumScale;
		perspectiveMatrix.at(2,2) = (fzFar + fzNear) / (fzNear - fzFar);
		perspectiveMatrix.at(2,3) = (2 * fzFar * fzNear) / (fzNear - fzFar);
		perspectiveMatrix.at(3,2) = -1.0f;

		program->use();
		program->uniform("perspectiveMatrix") = perspectiveMatrix;
		program->dispose();

		buffer = make_shared<gfx::GenericBuffer>();
		buffer->realloc({
			 0.25f,  0.25f, -1.25f, 1.0f,
			 0.25f, -0.25f, -1.25f, 1.0f,
			-0.25f,  0.25f, -1.25f, 1.0f,

			 0.25f, -0.25f, -1.25f, 1.0f,
			-0.25f, -0.25f, -1.25f, 1.0f,
			-0.25f,  0.25f, -1.25f, 1.0f,

			 0.25f,  0.25f, -2.75f, 1.0f,
			-0.25f,  0.25f, -2.75f, 1.0f,
			 0.25f, -0.25f, -2.75f, 1.0f,

			 0.25f, -0.25f, -2.75f, 1.0f,
			-0.25f,  0.25f, -2.75f, 1.0f,
			-0.25f, -0.25f, -2.75f, 1.0f,

			-0.25f,  0.25f, -1.25f, 1.0f,
			-0.25f, -0.25f, -1.25f, 1.0f,
			-0.25f, -0.25f, -2.75f, 1.0f,

			-0.25f,  0.25f, -1.25f, 1.0f,
			-0.25f, -0.25f, -2.75f, 1.0f,
			-0.25f,  0.25f, -2.75f, 1.0f,

			 0.25f,  0.25f, -1.25f, 1.0f,
			 0.25f, -0.25f, -2.75f, 1.0f,
			 0.25f, -0.25f, -1.25f, 1.0f,

			 0.25f,  0.25f, -1.25f, 1.0f,
			 0.25f,  0.25f, -2.75f, 1.0f,
			 0.25f, -0.25f, -2.75f, 1.0f,

			 0.25f,  0.25f, -2.75f, 1.0f,
			 0.25f,  0.25f, -1.25f, 1.0f,
			-0.25f,  0.25f, -1.25f, 1.0f,

			 0.25f,  0.25f, -2.75f, 1.0f,
			-0.25f,  0.25f, -1.25f, 1.0f,
			-0.25f,  0.25f, -2.75f, 1.0f,

			 0.25f, -0.25f, -2.75f, 1.0f,
			-0.25f, -0.25f, -1.25f, 1.0f,
			 0.25f, -0.25f, -1.25f, 1.0f,

			 0.25f, -0.25f, -2.75f, 1.0f,
			-0.25f, -0.25f, -2.75f, 1.0f,
			-0.25f, -0.25f, -1.25f, 1.0f,

			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,

			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,

			0.8f, 0.8f, 0.8f, 1.0f,
			0.8f, 0.8f, 0.8f, 1.0f,
			0.8f, 0.8f, 0.8f, 1.0f,

			0.8f, 0.8f, 0.8f, 1.0f,
			0.8f, 0.8f, 0.8f, 1.0f,
			0.8f, 0.8f, 0.8f, 1.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,

			0.5f, 0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f,

			0.5f, 0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f,

			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,

			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,

			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,

			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,

		});

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
	}

	virtual void onReshape(int width, int height) {

		perspectiveMatrix.at(0,0) = fFrustumScale / (width / (float)height);
		perspectiveMatrix.at(1,1) = fFrustumScale;

		program->use();
		program->uniform("perspectiveMatrix") = perspectiveMatrix;
		program->dispose();

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

		program->use();

		program->uniform("loopDuration") = 5.0f;
		program->uniform("time") = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

		buffer->bind(GL_ARRAY_BUFFER);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float)*buffer->size()/2));

		glDrawArrays(GL_TRIANGLES, 0, buffer->size()/2/4);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		buffer->unbind(GL_ARRAY_BUFFER);
		program->dispose();

		glutSwapBuffers();
		glutPostRedisplay();
	}

};

#endif /* APPLICATION_HPP_ */
