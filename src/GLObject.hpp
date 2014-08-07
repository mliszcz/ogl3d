/*
 * GLObject.hpp
 *
 *  Created on: Jul 29, 2014
 *      Author: michal
 */

#ifndef GLOBJECT_HPP_
#define GLOBJECT_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

class GLObject {

protected:
	GLuint _handle;

public:

	virtual ~GLObject() = default;

	GLuint handle() {
		return _handle;
	}

	operator GLuint() {
		return _handle;
	}

	// scoped bind/unbind
	virtual void bind() { }
	virtual void unbind() { }
};

class ContexObject {
public:
	virtual ~ContexObject() = default;

	template <typename... Args>
	void bind(Args... args) { }

	template <typename... Args>
	void unbind(Args... args) { }
};

class with {

private:

	function<void()> bind;
	function<void()> unbind;

public:

	template <typename T, typename... Args>
	with(shared_ptr<T> contextObject, Args... args)
		: with(contextObject.get(), args...) { }

	template <typename T, typename... Args>
	with(T* contextObject, Args... args) {

		// same trick at util::Singleton

		auto proxyBind = [contextObject](Args& ... args) {
			contextObject->bind(std::forward<Args&>(args) ...);
		};

		auto proxyUnbind = [contextObject](Args& ... args) {
			contextObject->unbind(std::forward<Args&>(args) ...);
		};

		bind = std::bind(proxyBind, std::ref(args)...);
		unbind = std::bind(proxyUnbind, std::ref(args)...);

		bind();
	}

	~with() {
		unbind();
	}
};

#endif /* GLOBJECT_HPP_ */
