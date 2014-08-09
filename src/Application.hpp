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
#include "util/HighResolutionClock.hpp"

#include "shader/Shader.hpp"
#include "shader/VertexShader.hpp"
#include "shader/FragmentShader.hpp"
#include "shader/Program.hpp"

#include "gfx/GenericBuffer.hpp"

#include "glm/glm.hpp"
#include "gli/gli.hpp"

#include "gfx/Camera.hpp"
#include "gfx/MatrixStack.hpp"
#include "gfx/Mesh.hpp"
#include "gfx/Texture.hpp"

#include "CarModel.hpp"

class Application : public ApplicationBase, public util::Singleton<Application> {
	friend class util::Singleton<Application>;

private:

	shared_ptr<shader::Program> progMaterialAds = nullptr;
	shared_ptr<shader::Program> progTextureAds = nullptr;
	vector<shared_ptr<shader::Program>> programs;

	util::HighResolutionClock hiResColck;

	gfx::Camera camera = gfx::Camera(glm::vec3(45.0f, -30.0f, 10.0f));

	shared_ptr<gfx::MatrixStack> modelToCameraStack = nullptr;

	glm::vec4 lightDirection = glm::vec4(0.866f, 0.5f, 0.0f, 0.0f);

	shared_ptr<CarModel> car = nullptr;
	shared_ptr<gfx::Mesh> plane = nullptr;
	shared_ptr<gfx::Mesh> skybox = nullptr;

	shared_ptr<gfx::Texture> texx = nullptr;

private:

	Application(int argc, char** argv)
		: ApplicationBase(argc, argv) { }

public:

	virtual ~Application() { }

	GLuint g_textureSampler = 0;

	virtual void onInit() {

		glGenSamplers(1, &g_textureSampler);
		glSamplerParameteri(g_textureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(g_textureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameterf(g_textureSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
		glSamplerParameteri(g_textureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(g_textureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

		modelToCameraStack = make_shared<gfx::MatrixStack>();

		car = make_shared<CarModel>(gfx::Mesh::fromObjFile("res/models/mustang_triang/mustang_triang.obj"));
		plane = gfx::Mesh::fromObjFile("res/models/plane/plane.obj");
		skybox = gfx::Mesh::fromObjFile("res/models/skybox/skybox.obj");

		progMaterialAds = shared_ptr<shader::Program>(new shader::Program({
			shader::VertexShader::fromFile("res/shaders/pos-norm-passthrough.vert"),
			shader::FragmentShader::fromFile("res/shaders/material-ads-lighting.frag")
		}));

		progTextureAds = shared_ptr<shader::Program>(new shader::Program({
			shader::VertexShader::fromFile("res/shaders/pos-norm-tex-passthrough.vert"),
			shader::FragmentShader::fromFile("res/shaders/texture-ads-lighting.frag")
		}));

		programs.push_back(progMaterialAds);
		programs.push_back(progTextureAds);

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

		glm::mat4 projection = glm::perspectiveFov(45.0f, (float)width, (float)height, 1.0f, 100.0f);

		for (auto& program : programs) {
			program->use();
			program->uniform("cameraToClipMatrix") = projection;
			program->dispose();
		}

		glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	}

	void onKeyDown(unsigned char key, int x, int y) {
		switch (key) {
		case 27:
			glutLeaveMainLoop();
			return;

		case 'w': car->accelerate(true); break;
		case 's': car->decelerate(true); break;
		case 'a': car->steerLeft(true); break;
		case 'd': car->steerRight(true); break;
		}

		glutPostRedisplay();
	}

	void onKeyUp(unsigned char key, int x, int y) {
		switch (key) {
		case 'w': car->accelerate(false); break;
		case 's': car->decelerate(false); break;
		case 'a': car->steerLeft(false); break;
		case 'd': car->steerRight(false); break;
		}
		glutPostRedisplay();
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

				camera.position.z = glm::clamp(camera.position.z, 10.0f, 50.0f);
		}

		glutPostRedisplay();
	}

	virtual void onMotion(int x, int y) {

		if (dragging) {

			camera.position.y -= (y-y0)/1.0f;
			camera.position.x += (x-x0)/1.0f;

//			camera.position.y = glm::clamp(camera.position.y, -78.75f, -15.0f);

			x0 = x;
			y0 = y;

			glutPostRedisplay();
		}
	}

	virtual void onDisplay() {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec4 worldLightPos = 10.0f*glm::vec4(10.0f, 5.0f, 10.0f, 1.0f);

		float frameTime = hiResColck.delta();
		car->update(frameTime);

		modelToCameraStack->set(camera.lookAt(car->position()));

		glm::vec4 cameraLightPos = modelToCameraStack->top() * worldLightPos;

		{ with program(progMaterialAds);

			progMaterialAds->uniform("cameraSpaceLightPos") = cameraLightPos;

			progMaterialAds->uniform("lightIntensity") = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
			progMaterialAds->uniform("ambientIntensity") = 0.5f*glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

			{ with matrixStack(modelToCameraStack);

				modelToCameraStack->scale(20.0f, 20.f, 20.f);

				progMaterialAds->uniform("modelToCameraMatrix") = modelToCameraStack->top();
				progMaterialAds->uniform("cameraToModelMatrix") = glm::inverse(modelToCameraStack->top());

				skybox->drawAll();
			}

			car->draw(progMaterialAds, modelToCameraStack);
		}


		{ with program(progTextureAds);

			progMaterialAds->uniform("cameraSpaceLightPos") = cameraLightPos;

			modelToCameraStack->scale(1000.0f, 1.0f, 1000.0f);
			progTextureAds->uniform("lightIntensity") = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
			progTextureAds->uniform("ambientIntensity") = 0.5f*glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

			{ with texture(plane->at(0).second.material()->mapKd);

				progTextureAds->uniform("modelToCameraMatrix") = modelToCameraStack->top();
				progTextureAds->uniform("cameraToModelMatrix") = glm::inverse(modelToCameraStack->top());

				plane->drawAll();
			}
		}

		util::CheckError();

		glutSwapBuffers();

		glutPostRedisplay();

//		static bool once = true;
//		if(once) glutPostRedisplay();
//		once = false;
	}

};

#endif /* APPLICATION_HPP_ */
