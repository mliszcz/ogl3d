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

#include "glm/glm.hpp"

#include "gfx/Camera.hpp"
#include "gfx/MatrixStack.hpp"
#include "gfx/Mesh.hpp"

class Application : public ApplicationBase, public util::Singleton<Application> {
	friend class util::Singleton<Application>;

private:

	shared_ptr<shader::Program> program = nullptr;

	gfx::MatrixStack modelToCameraStack;

	shared_ptr<gfx::Mesh> car = nullptr;

	gfx::Camera camera = gfx::Camera(
			glm::vec3(90.0f, 0.0f, 10.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));

	glm::fquat g_orientation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec4 lightDirection = glm::vec4(0.866f, 0.5f, 0.0f, 0.0f);

private:


	void OffsetOrientation(const glm::vec3 &_axis, float fAngDeg)
	{
		float fAngRad = util::DegToRad(fAngDeg);

		glm::vec3 axis = glm::normalize(_axis);

		axis = axis * sinf(fAngRad / 2.0f);
		float scalar = cosf(fAngRad / 2.0f);

		glm::fquat offset(scalar, axis.x, axis.y, axis.z);

//		switch(g_iOffset)
//		{
//		case MODEL_RELATIVE:
			g_orientation = g_orientation * offset;
//			break;
//		case WORLD_RELATIVE:
//			g_orientation = offset * g_orientation;
//			break;
//		case CAMERA_RELATIVE:
//			{
//				const glm::vec3 &camPos = ResolveCamPosition();
//				const glm::mat4 &camMat = CalcLookAtMatrix(camPos, g_camTarget, glm::vec3(0.0f, 1.0f, 0.0f));
//
//				glm::fquat viewQuat = glm::quat_cast(camMat);
//				glm::fquat invViewQuat = glm::conjugate(viewQuat);
//
//				const glm::fquat &worldQuat = (invViewQuat * offset * viewQuat);
//				g_orientation = worldQuat * g_orientation;
//			}
//			break;
//		}

		g_orientation = glm::normalize(g_orientation);
	}

	Application(int argc, char** argv)
		: ApplicationBase(argc, argv) { }

public:

	virtual ~Application() { }

	virtual void onInit() {

		car = gfx::Mesh::fromObjFile("res/models/mustang_triang/mustang_triang.obj");



		auto shaders = {
				shader::VertexShader::fromFile("res/shaders/simple.vert"),
				shader::FragmentShader::fromFile("res/shaders/simple.frag")
		};

		program = make_shared<shader::Program>(shaders);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f); // Z-mapping from NDC to window-space
		glEnable(GL_DEPTH_CLAMP);
	}

	virtual void onReshape(int width, int height) {

		program->use();
		program->uniform("cameraToClipMatrix") =
				glm::perspectiveFov(45.0f, (float)width, (float)height, 1.0f, 100.0f);
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

	int x0 = 0;
	int y0 = 0;
	bool dragging = false;
	virtual void onMouse(int button, int state, int x, int y) {

		static int GLUT_WHEEL_UP = 3;
		static int GLUT_WHEEL_DOWN = 4;

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			x0 = x;
			y0 = y;
			dragging = true;
		}

		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
			dragging = false;

		if (state == GLUT_UP ) {
				if (button == GLUT_WHEEL_UP)
					camera.position.z -= 1.0f;
				else if(button == GLUT_WHEEL_DOWN)
					camera.position.z += 1.0f;
		}

		glutPostRedisplay();
	}

	virtual void onMotion(int x, int y) {

		if (dragging) {

			camera.position.y -= (y-y0)/1.0f;
			camera.position.x += (x-x0)/1.0f;

			glm::clamp(camera.position.y, -78.75f, 0.0f);

			x0 = x;
			y0 = y;

			glutPostRedisplay();
		}
	}

	virtual void onDisplay() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelToCameraStack.set(camera.calculateLookAtMatrix());
		modelToCameraStack.translate(camera.target);
		modelToCameraStack.apply(glm::mat4_cast(g_orientation));

		glm::vec4 lightDirCameraSpace = modelToCameraStack.top() * lightDirection;

		program->use();

		program->uniform("dirToLight") = glm::vec3(lightDirCameraSpace);
		program->uniform("modelToCameraMatrix") = modelToCameraStack.top();
		program->uniform("normalModelToCameraMatrix") = glm::mat3(modelToCameraStack.top());
		program->uniform("lightIntensity") = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		program->uniform("ambientIntensity") = 0.5f*glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

		for (unsigned int i=0; i<car->size(); ++i) {
			car->at(i).bindVAO();
			car->at(i).draw(program);
			car->at(i).unbindVAO();
		}
//		car->drawAll(program);

		program->dispose();

		util::CheckError();

		glutSwapBuffers();

		static bool once = true;
		if(once) glutPostRedisplay();
		once = false;
	}

};

#endif /* APPLICATION_HPP_ */
