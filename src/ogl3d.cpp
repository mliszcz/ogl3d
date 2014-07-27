//============================================================================
// Name        : ogl3d.cpp
// Author      : michal
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Common.hpp"

#include "util/Config.hpp"
#include "util/Logger.hpp"

#include "shader/VertexShader.hpp"
#include "shader/FragmentShader.hpp"
#include "shader/Shader.hpp"
#include "shader/Program.hpp"

#include "Application.hpp"

int main(int argc, char** argv) {

	auto config = util::Config::getInstance("res/ogl3d.config");
	auto logLvl = util::Logger::LoggerLevel::INFO;
	auto logger = util::Logger::getInstance(std::cout, logLvl);

	auto app = Application::getInstance(argc, argv);

	app->onInit();
	app->run();

	return 0;
}

