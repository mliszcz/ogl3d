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

#include "gfx/MatrixStack.hpp"
#include "gfx/Mesh.hpp"

#include "Hierarchy.hpp"

class Application : public ApplicationBase, public util::Singleton<Application> {
	friend class util::Singleton<Application>;

private:

	shared_ptr<shader::Program> program = nullptr;

	shared_ptr<gfx::Mesh> mesh1 = nullptr;
	shared_ptr<gfx::Mesh> mesh2 = nullptr;

	gfx::MatrixStack modelToCameraStack;

	shared_ptr<gfx::Mesh> hierarchyMesh = nullptr;

	shared_ptr<Hierarchy> g_armature = nullptr;

	shared_ptr<gfx::Mesh> car = nullptr;


private:

	Application(int argc, char** argv)
		: ApplicationBase(argc, argv) { }

public:

	virtual ~Application() { }

	virtual void onInit() {

		modelToCameraStack.translate(0, 0, -30);
		modelToCameraStack.scale(0.2, 0.2, 0.2);

		car = gfx::Mesh::fromObjFile("res/models/BMW/BMW_obj.obj");
//		car = gfx::Mesh::fromObjFile("res/models/mustang/mustang.obj");

		mesh1 = gfx::Mesh::fromFile("res/models/model01.mesh");
		mesh2 = gfx::Mesh::fromFile("res/models/model02.mesh");
		hierarchyMesh = gfx::Mesh::fromFile("res/models/hierarchy.mesh");

		auto shaders = {
				shader::VertexShader::fromFile("res/shaders/simple.vert"),
				shader::FragmentShader::fromFile("res/shaders/simple.frag")
		};

		program = make_shared<shader::Program>(shaders);

//		g_armature = make_shared<Hierarchy>(program, hierarchyMesh);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f); // Z-mapping from NDC to window-space
//		glEnable(GL_DEPTH_CLAMP);
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
		case 'a': g_armature->AdjBase(true); break;
		case 'd': g_armature->AdjBase(false); break;
		case 'w': g_armature->AdjUpperArm(false); break;
		case 's': g_armature->AdjUpperArm(true); break;
		case 'r': g_armature->AdjLowerArm(false); break;
		case 'f': g_armature->AdjLowerArm(true); break;
		case 't': g_armature->AdjWristPitch(false); break;
		case 'g': g_armature->AdjWristPitch(true); break;
		case 'z': g_armature->AdjWristRoll(true); break;
		case 'c': g_armature->AdjWristRoll(false); break;
		case 'q': g_armature->AdjFingerOpen(true); break;
		case 'e': g_armature->AdjFingerOpen(false); break;
		case 32: g_armature->WritePose(); break;
		}

		glutPostRedisplay();
	}

	virtual void onDisplay() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//		g_armature->Draw();

		program->use();

		program->uniform("modelToCameraMatrix") = modelToCameraStack.top();
		car->drawAll();

		program->dispose();

		util::CheckError();

		glutSwapBuffers();

		static bool once = true;
		if(once) glutPostRedisplay();
		once = false;
	}

};

#endif /* APPLICATION_HPP_ */
