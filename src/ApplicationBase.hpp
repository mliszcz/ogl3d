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
	virtual void onKeyDown(unsigned char key, int x, int y) { }
	virtual void onKeyUp(unsigned char key, int x, int y) { }
	virtual void onSpecialKeyDown(int key, int x, int y) { }
	virtual void onSpecialKeyUp(int key, int x, int y) { }
	virtual void onMouse(int button, int state, int x, int y) { }
	virtual void onMotion(int x, int y) { }
	virtual void onPassiveMotion(int x, int y) { }

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

		if (res != GLEW_OK)
			throw logic_error("GLEW error: " +
					string(reinterpret_cast<const char*>(glewGetErrorString(res))));

		/*
		 * empty OpenGL error flag
		 * it is raised due to glewExperimental set to TRUE
		 * and can be safely ignored
		 * http://stackoverflow.com/questions/10857335/opengl-glgeterror-returns-invalid-enum-after-call-to-glewinit
		 */
		glGetError();

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
		static std::function<void(unsigned char, int, int)>	fun_keyUp;
		static std::function<void(unsigned char, int, int)>	fun_keyDown;
		static std::function<void(int, int, int)>			fun_specKeyUp;
		static std::function<void(int, int, int)>			fun_specKeyDown;
		static std::function<void(int, int, int, int)>		fun_mouse;
		static std::function<void(int, int)>				fun_motion;
		static std::function<void(int, int)>				fun_passiveMotion;

		fun_display = [this]() { this->onDisplay(); };
		fun_reshape = [this](int w, int h) { this->onReshape(w, h); };
		fun_keyDown = [this](unsigned char key, int x, int y) { this->onKeyDown(key, x, y); };
		fun_keyUp = [this](unsigned char key, int x, int y) { this->onKeyUp(key, x, y); };
		fun_specKeyDown = [this](int key, int x, int y) { this->onSpecialKeyDown(key, x, y); };
		fun_specKeyUp = [this](int key, int x, int y) { this->onSpecialKeyUp(key, x, y); };
		fun_mouse = [this](int btn, int state, int x, int y) { this->onMouse(btn, state, x, y); };
		fun_motion = [this](int x, int y) { this->onMotion(x, y); };
		fun_passiveMotion = [this](int x, int y) { this->onPassiveMotion(x, y); };

		glutDisplayFunc([](){ fun_display(); });
		glutReshapeFunc([](int w, int h){ fun_reshape(w, h); });
		glutKeyboardFunc([](unsigned char key, int x, int y){ fun_keyDown(key, x, y); });
		glutKeyboardUpFunc([](unsigned char key, int x, int y){ fun_keyUp(key, x, y); });
		glutSpecialFunc([](int key, int x, int y){ fun_keyDown(key, x, y); });
		glutSpecialUpFunc([](int key, int x, int y){ fun_keyUp(key, x, y); });
		glutMouseFunc([](int btn, int state, int x, int y){ fun_mouse(btn, state, x, y); });
		glutMotionFunc([](int x, int y){ fun_motion(x, y); });
		glutPassiveMotionFunc([](int x, int y){ fun_passiveMotion(x, y); });
	}

};

#endif /* APPLICATIONBASE_HPP_ */
