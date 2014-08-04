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

	glm::vec3 g_camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::fquat g_orientation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 g_sphereCamRelPos = glm::vec3(90.0f, 0.0f, 10.0f);


private:

	glm::vec3 ResolveCamPosition()
	{
		gfx::MatrixStack tempMat;

		float phi = util::DegToRad(g_sphereCamRelPos.x);
		float theta = util::DegToRad(g_sphereCamRelPos.y + 90.0f);

		float fSinTheta = sinf(theta);
		float fCosTheta = cosf(theta);
		float fCosPhi = cosf(phi);
		float fSinPhi = sinf(phi);

		glm::vec3 dirToCamera(fSinTheta * fCosPhi, fCosTheta, fSinTheta * fSinPhi);
		return (dirToCamera * g_sphereCamRelPos.z) + g_camTarget;
	}

	glm::mat4 CalcLookAtMatrix(const glm::vec3 &cameraPt, const glm::vec3 &lookPt, const glm::vec3 &upPt)
	{
		glm::vec3 lookDir = glm::normalize(lookPt - cameraPt);
		glm::vec3 upDir = glm::normalize(upPt);

		glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
		glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

		glm::mat4 rotMat(1.0f);
		rotMat[0] = glm::vec4(rightDir, 0.0f);
		rotMat[1] = glm::vec4(perpUpDir, 0.0f);
		rotMat[2] = glm::vec4(-lookDir, 0.0f);

		rotMat = glm::transpose(rotMat);

		glm::mat4 transMat(1.0f);
		transMat[3] = glm::vec4(-cameraPt, 1.0f);

		return rotMat * transMat;
	}

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
//			g_orientation = g_orientation * offset;
//			break;
//		case WORLD_RELATIVE:
			g_orientation = offset * g_orientation;
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

		modelToCameraStack.translate(0, 0, -2);
//		modelToCameraStack.scale(0.2, 0.2, 0.2);

//		car = gfx::Mesh::fromObjFile("res/models/BMW/BMW_obj.obj");
		car = gfx::Mesh::fromObjFile("res/models/mustang/mustang.obj");

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

//		case 'a': g_armature->AdjBase(true); break;
//		case 'd': g_armature->AdjBase(false); break;
//		case 'w': g_armature->AdjUpperArm(false); break;
//		case 's': g_armature->AdjUpperArm(true); break;
//		case 'r': g_armature->AdjLowerArm(false); break;
//		case 'f': g_armature->AdjLowerArm(true); break;
//		case 't': g_armature->AdjWristPitch(false); break;
//		case 'g': g_armature->AdjWristPitch(true); break;
//		case 'z': g_armature->AdjWristRoll(true); break;
//		case 'c': g_armature->AdjWristRoll(false); break;
//		case 'q': g_armature->AdjFingerOpen(true); break;
//		case 'e': g_armature->AdjFingerOpen(false); break;
//		case 32: g_armature->WritePose(); break;

		case 'i': g_sphereCamRelPos.y -= 11.25f; break;
		case 'k': g_sphereCamRelPos.y += 11.25f; break;
		case 'j': g_sphereCamRelPos.x -= 11.25f; break;
		case 'l': g_sphereCamRelPos.x += 11.25f; break;
		case 'I': g_sphereCamRelPos.y -= 1.125f; break;
		case 'K': g_sphereCamRelPos.y += 1.125f; break;
		case 'J': g_sphereCamRelPos.x -= 1.125f; break;
		case 'L': g_sphereCamRelPos.x += 1.125f; break;
		}

		g_sphereCamRelPos.y = glm::clamp(g_sphereCamRelPos.y, -78.75f, 10.0f);

		glutPostRedisplay();
	}

	virtual void onDisplay() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const glm::vec3 &camPos = ResolveCamPosition();
		modelToCameraStack.set(CalcLookAtMatrix(camPos, g_camTarget, glm::vec3(0.0f, 1.0f, 0.0f)));
		modelToCameraStack.translate(g_camTarget);
		modelToCameraStack.apply(glm::mat4_cast(g_orientation));
//		modelToCameraStack.rotateX(-90);
//		g_armature->Draw();

		program->use();

		program->uniform("modelToCameraMatrix") = modelToCameraStack.top();

		for (unsigned int i=2; i<car->size(); ++i) {
			car->at(i).bindVAO();
			car->at(i).draw();
			car->at(i).unbindVAO();
		}
//		car->drawAll();

		program->dispose();

		util::CheckError();

		glutSwapBuffers();

		static bool once = true;
		if(once) glutPostRedisplay();
		once = false;
	}

};

#endif /* APPLICATION_HPP_ */
