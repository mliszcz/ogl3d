/*
 * HighResolutionClock.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: michal
 */

#ifndef HIGHRESOLUTIONCLOCK_HPP_
#define HIGHRESOLUTIONCLOCK_HPP_

#include "../Common.hpp"
#include <chrono>

namespace util {

using std::chrono::high_resolution_clock;
using std::chrono::duration;

class HighResolutionClock {

private:
	high_resolution_clock::time_point lastCheck;

public:

	HighResolutionClock()
		: lastCheck(high_resolution_clock::now()) { }

	float delta() {
		high_resolution_clock::time_point newCheck = high_resolution_clock::now();
		duration<float> durationSeconds = newCheck - lastCheck;
		lastCheck = newCheck;
		return durationSeconds.count();
	}
};

}

#endif /* HIGHRESOLUTIONCLOCK_HPP_ */
