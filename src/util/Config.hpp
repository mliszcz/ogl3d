/*
 * Config.hpp
 *
 *  Created on: Jul 26, 2014
 *      Author: michal
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <fstream>
#include "../Common.hpp"
#include "Singleton.hpp"

namespace util {

class Config : public Singleton<Config> {
	friend class Singleton;

private:

	map<string, int> options;

	Config(const string& configFilePath) {
		std::ifstream input(configFilePath.c_str());
		for (string line; std::getline(input, line);) {
			size_t delim = line.find('=');
			options[line.substr(0, delim)] = atoi(line.substr(delim + 1).c_str());
		}
	}

public:

	int get(string name) {
		return options[name];
	}

};

}

#endif /* CONFIG_HPP_ */
