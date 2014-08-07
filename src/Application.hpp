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

	gfx::Camera camera = gfx::Camera(
			glm::vec3(45.0f, -30.0f, 10.0f),
			glm::vec3(0.0f, 0.0f, 0.0f));

	shared_ptr<gfx::MatrixStack> modelToCameraStack = nullptr;

	glm::fquat g_orientation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec4 lightDirection = glm::vec4(0.866f, 0.5f, 0.0f, 0.0f);

	shared_ptr<CarModel> car = nullptr;
	shared_ptr<gfx::Mesh> plane = nullptr;
	shared_ptr<gfx::Mesh> skybox = nullptr;

	shared_ptr<gfx::Texture> texx = nullptr;

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

	GLuint g_textureSampler = 0;
	GLuint g_shineTexUnit = 0;	// just some arbitrary number
	GLuint g_shineTexture = 0;

	virtual ~Application() { }

	void loadTexture(string ddsFile) {
		gli::texture2D Texture(gli::load_dds(ddsFile.c_str()));
		assert(!Texture.empty());
		glBindTexture(GL_TEXTURE_2D, g_shineTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(Texture.levels() - 1));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		glTexStorage2D(GL_TEXTURE_2D, GLint(Texture.levels()), GLenum(gli::internal_format(Texture.format())), GLsizei(Texture.dimensions().x), GLsizei(Texture.dimensions().y));
		if (!gli::is_compressed(Texture.format())) {
			for (gli::texture2D::size_type Level = 0; Level < Texture.levels(); ++Level) {
				gli::image tex = Texture[Level];
				glTexSubImage2D(GL_TEXTURE_2D, GLint(Level), 0, 0,
						GLsizei(Texture[Level].dimensions().x),
						GLsizei(Texture[Level].dimensions().y),
						GLenum(gli::external_format(Texture.format())),
						GLenum(gli::type_format(Texture.format())),
						Texture[Level].data());
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	virtual void onInit() {

		glGenSamplers(1, &g_textureSampler);
		glSamplerParameteri(g_textureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glSamplerParameteri(g_textureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameterf(g_textureSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
		glSamplerParameteri(g_textureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(g_textureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

//		texx = gfx::Texture::fromDdsFile("res/models/plane/asphalt_9_512x512.dds");

		glGenTextures(1, &g_shineTexture);

		loadTexture("res/models/plane/asphalt_9_512x512.dds");

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

	float dir = 0.0f;
	void onKeyboard(unsigned char key, int x, int y) {
		switch (key) {
		case 27:
			glutLeaveMainLoop();
			return;

		case 'a': car->steerLeft(); break;
		case 'd': car->steerRight(); break;
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

		modelToCameraStack->set(camera.calculateLookAtMatrix());
//		modelToCameraStack->translate(0.0f, -1.0f, 0.0f);
		modelToCameraStack->translate(camera.target);
//		modelToCameraStack.rotateY(dir);
//		modelToCameraStack->apply(glm::mat4_cast(g_orientation));

//		glm::vec4 lightDirCameraSpace = modelToCameraStack.top() * lightDirection;

		progMaterialAds->use();
		progMaterialAds->uniform("modelSpaceLightPos") = 10.0f*glm::vec3(0.5f, 1.0f, 0.5f);
//		program->uniform("dirToLight") = glm::vec3(lightDirCameraSpace);
		progMaterialAds->uniform("modelToCameraMatrix") = modelToCameraStack->top();
//		program->uniform("normalModelToCameraMatrix") = glm::mat3(modelToCameraStack.top());
		progMaterialAds->uniform("lightIntensity") = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		progMaterialAds->uniform("ambientIntensity") = 0.5f*glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

		modelToCameraStack->push();
		modelToCameraStack->scale(20.0f, 20.f, 20.f);
		progMaterialAds->uniform("modelToCameraMatrix") = modelToCameraStack->top();

		skybox->drawAll();
		modelToCameraStack->pop();

		progMaterialAds->uniform("modelToCameraMatrix") = modelToCameraStack->top();

		car->draw(progMaterialAds, modelToCameraStack);


		progMaterialAds->dispose();



		progTextureAds->use();

		modelToCameraStack->scale(1000.0f, 1.0f, 1000.0f);
		progTextureAds->uniform("modelToCameraMatrix") = modelToCameraStack->top();

		progTextureAds->uniform("modelSpaceLightPos") = 10.0f*glm::vec3(0.5f, 1.0f, 0.5f);
		//		program->uniform("dirToLight") = glm::vec3(lightDirCameraSpace);
		progTextureAds->uniform("modelToCameraMatrix") = modelToCameraStack->top();
		//		program->uniform("normalModelToCameraMatrix") = glm::mat3(modelToCameraStack.top());
		progTextureAds->uniform("lightIntensity") = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		progTextureAds->uniform("ambientIntensity") = 0.5f*glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

		GLuint gaussianTextureUnif = glGetUniformLocation(progTextureAds->handle(), "textureSampler");
		glUniform1i(gaussianTextureUnif, g_shineTexUnit);


		plane->at(0).second.material()->mapKd->bind();
		plane->drawAll();
		plane->at(0).second.material()->mapKd->unbind();

		progTextureAds->dispose();



		util::CheckError();

		glutSwapBuffers();

		static bool once = true;
		if(once) glutPostRedisplay();
		once = false;
	}

};

#endif /* APPLICATION_HPP_ */
