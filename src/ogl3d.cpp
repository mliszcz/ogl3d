//============================================================================
// Name        : ogl3d.cpp
// Author      : michal
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Common.hpp"
#include "util/Configuration.hpp"
#include "util/Logger.hpp"


int main() {

	auto config = util::Configuration::getInstance("res/ogl3d.config");
	auto logger = util::Logger::getInstance(std::cout);

	logger->info("fs: %d\n", config->get("FULLSCREEN"));

	return 0;
}
