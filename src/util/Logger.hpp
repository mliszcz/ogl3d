/*
 * Logger.hpp
 *
 *  Created on: Jul 26, 2014
 *      Author: michal
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <cstdio>
#include <iostream>
#include "../Common.hpp"
#include "Singleton.hpp"

namespace util {

class Logger: public Singleton<Logger> {
	friend class Singleton;

private:

	std::ostream& out;
	char buffer[512];

	Logger(std::ostream& loggerOutput) :
		out(loggerOutput) { }

public:

	template <typename ... Args>
	void info(string format, Args ... args) {
		sprintf(buffer, format.c_str(), std::forward<Args>(args) ...);
		out << buffer;
	}

	template <typename ... Args>
	void warn(string format, Args ... args) {
		sprintf(buffer, format.c_str(), std::forward<Args>(args) ...);
		out << buffer;
	}

	template <typename ... Args>
	void error(string format, Args ... args) {
		sprintf(buffer, format.c_str(), std::forward<Args>(args) ...);
		out << buffer;
	}
};

}

#endif /* LOGGER_HPP_ */
