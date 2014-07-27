/*
 * Singleton.hpp
 *
 *  Created on: Jul 26, 2014
 *      Author: michal
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <functional>

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

	/*
	 * getInstance() is created for each Args set, at compile time
	 * allowing to have multiple instances when called with diffrent arguments
	 *
	 * the solution is not to pass Args to the function where static instance
	 * is created - construct(f). Args are passed in f closure
	 */

	template <typename ... Args>
	static shared_ptr<T> getInstance(Args& ... args) {

		/*
		 * we cant od this, cause there will be multiple copies of getInstance(...)
		 * each with its own static variable 'instance'
		 */
//		static shared_ptr<T> instance = shared_ptr<T>(new T(std::forward<Args&>(args) ...));
//		return instance;

		/*
		 * we cant do this, cause gcc 4.8.2 is fucked up - it is not possible to capture variadic args (i 4.9 this is fixed)
		 * http://binglongx.wordpress.com/2014/04/25/workaround-for-gcc-bug-in-lambda-capturing-variadic-templates/
		 */
//		return construct([&](){
//			return new T(std::forward<Args&>(args) ...);
//		});

		auto constructor = [](Args& ... args) {
			return new T(std::forward<Args&>(args) ...);
		};

		auto boundConstructor = std::bind(constructor, std::ref(args)...);

		return construct(boundConstructor);
	}

private:

	static shared_ptr<T> construct(std::function<T*()> constructor) {
		static shared_ptr<T> instance = shared_ptr<T>(constructor());
		return instance;
	}
};

}

#endif /* SINGLETON_HPP_ */
