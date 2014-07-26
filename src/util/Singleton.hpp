/*
 * Singleton.hpp
 *
 *  Created on: Jul 26, 2014
 *      Author: michal
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "../Common.hpp"

namespace util {

template <typename T>
class Singleton {

protected:

	Singleton() { }
	Singleton(const Singleton&) = delete;
	void operator=(const Singleton&) = delete;
	virtual ~Singleton() { }

public:

	template <typename ... Args>
	static shared_ptr<T> getInstance(Args& ... args) {
		static shared_ptr<T> instance = shared_ptr<T>(new T(std::forward<Args&>(args) ...));
		return instance;
	}
};

}

#endif /* SINGLETON_HPP_ */
