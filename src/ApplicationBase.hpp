/*
 * ApplicationBase.hpp
 *
 *  Created on: Jul 27, 2014
 *      Author: michal
 */

#ifndef APPLICATIONBASE_HPP_
#define APPLICATIONBASE_HPP_

#include <functional>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Common.hpp"

#include "util/Config.hpp"
#include "util/Logger.hpp"

#include "shader/Shader.hpp"
#include "shader/VertexShader.hpp"
#include "shader/FragmentShader.hpp"
#include "shader/Program.hpp"

#include "gfx/GenericBuffer.hpp"


class ApplicationBase {

private:

	shared_ptr<util::Config> config = util::Config::getInstance();
	shared_ptr<util::Logger> logger = util::Logger::getInstance();

public:

	virtual void onInit() = 0;
	virtual void onDisplay() = 0;
	virtual void onReshape(int width, int height) = 0;
	virtual void onKeyboard(unsigned char key, int x, int y) = 0;

	void run() {
		glutMainLoop();
	}

	virtual ~ApplicationBase() = default;

protected:

	ApplicationBase(int argc, char** argv) {

		glutInit(&argc, argv);

		int width = config->get("WINDOW_WIDTH");
		int height = config->get("WINDOW_HEIGHT");

		unsigned int displayMode = GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL;

		glutInitDisplayMode(displayMode);
		glutInitContextVersion(3, 3);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitContextFlags(GLUT_DEBUG);

		glutInitWindowSize(width, height);
		glutCreateWindow(argv[0]);

		glewExperimental = GL_TRUE; // required on i5-4200u/HD4400 & Mesa 10.1.3
		GLenum res = glewInit();
		if (res != GLEW_OK) {
			fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		}

//		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

		/*
		 * static wrappers are needed, cause opengl is crappy
		 *
		 * lambda with captured state cannot be used as plain old c-style function
		 *
		 * there can be only one instance of Application
		 * but this is not a problem, cause Application is singleton
		 */
		static std::function<void()>						fun_display;
		static std::function<void(int, int)>				fun_reshape;
		static std::function<void(unsigned char, int, int)>	fun_keyboard;

		fun_display = [this]() { this->onDisplay(); };
		fun_reshape = [this](int w, int h) { this->onReshape(w, h); };
		fun_keyboard = [this](unsigned char key, int x, int y) { this->onKeyboard(key, x, y); };

		glutDisplayFunc([](){ fun_display(); });
		glutReshapeFunc([](int w, int h){ fun_reshape(w, h); });
		glutKeyboardFunc([](unsigned char key, int x, int y){ fun_keyboard(key, x, y); });
	}

};

#endif /* APPLICATIONBASE_HPP_ */
