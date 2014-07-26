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

public:
	enum LoggerLevel {
		INFO, WARN, ERROR, NONE
	};

private:

	LoggerLevel level;
	std::ostream& out;
	char buffer[512];

	Logger(std::ostream& loggerOutput, const LoggerLevel& loggerLevel)
		: level(loggerLevel), out(loggerOutput) { }

public:

	template <typename ... Args>
	void info(string format, Args ... args) {
		if(level > LoggerLevel::INFO) return;
		sprintf(buffer, format.c_str(), std::forward<Args>(args) ...);
		out << buffer;
	}

	template <typename ... Args>
	void warn(string format, Args ... args) {
		if(level > LoggerLevel::WARN) return;
		sprintf(buffer, format.c_str(), std::forward<Args>(args) ...);
		out << buffer;
	}

	template <typename ... Args>
	void error(string format, Args ... args) {
		if(level > LoggerLevel::ERROR) return;
		sprintf(buffer, format.c_str(), std::forward<Args>(args) ...);
		out << buffer;
	}
};

}

#endif /* LOGGER_HPP_ */
