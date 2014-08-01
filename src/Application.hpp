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

#include "gfx/Mesh.hpp"

class Application : public ApplicationBase, public util::Singleton<Application> {
	friend class util::Singleton<Application>;

private:

	shared_ptr<shader::Program> program = nullptr;

//	GLuint vao = 0;

	gfx::Matrix<4, 4, float> perspectiveMatrix;
	float fFrustumScale = 1.0f;

	shared_ptr<gfx::Mesh> mesh1 = nullptr;
	shared_ptr<gfx::Mesh> mesh2 = nullptr;

private:

	Application(int argc, char** argv)
		: ApplicationBase(argc, argv) { }

public:

	virtual ~Application() { }

	virtual void onInit() {


		mesh1 = gfx::Mesh::fromFile("res/models/model01.mesh");
		mesh2 = gfx::Mesh::fromFile("res/models/model02.mesh");

		auto shaders = {
				shader::VertexShader::fromFile("res/shaders/simple.vert"),
				shader::FragmentShader::fromFile("res/shaders/simple.frag")
		};

		program = make_shared<shader::Program>(shaders);



		float fzNear = 1.0f; float fzFar = 3.0f;

		perspectiveMatrix.at(0,0) = fFrustumScale;
		perspectiveMatrix.at(1,1) = fFrustumScale;
		perspectiveMatrix.at(2,2) = (fzFar + fzNear) / (fzNear - fzFar);
		perspectiveMatrix.at(2,3) = (2 * fzFar * fzNear) / (fzNear - fzFar);
		perspectiveMatrix.at(3,2) = -1.0f;

		program->use();
		program->uniform("perspectiveMatrix") = perspectiveMatrix;
		program->dispose();

//		glGenVertexArrays(1, &vao);
//		glBindVertexArray(vao);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDepthRange(0.0f, 1.0f); // Z-mapping from NDC to window-space
	}

	virtual void onReshape(int width, int height) {

		perspectiveMatrix.at(0,0) = fFrustumScale / ((float) width/height);
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
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program->use();

		mesh1->bindVAO();
		program->uniform("offset") = gfx::Vector<3, float>{0.0f, 0.0f, 0.0f};
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_SHORT, 0);

		mesh2->bindVAO();
		program->uniform("offset") = gfx::Vector<3, float>{0.0f, 0.0f, -1.0f};
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_SHORT, 0);

		mesh2->unbindVAO();

		program->dispose();

		glutSwapBuffers();

		static bool once = true;
		if(once) glutPostRedisplay();
		once = false;
	}

};

#endif /* APPLICATION_HPP_ */
