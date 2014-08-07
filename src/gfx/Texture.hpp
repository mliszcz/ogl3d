/*
 * Texture.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: michal
 */

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gli/gli.hpp"

#include "../Common.hpp"

namespace gfx {

class Texture : public GLObject {

private:

	GLuint _unit = 0;

	Texture(GLuint handle, GLuint unit) {
		_handle = handle;
		_unit = unit;
	}

public:

	void bind() {
		glActiveTexture(GL_TEXTURE0 + _unit);
		glBindTexture(GL_TEXTURE_2D, _handle);

		// do i need to bind it to sampler??
//		glBindSampler(_unit, texSampler);
	}

	void unbind() {
		glBindSampler(_unit, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	static shared_ptr<Texture> fromDdsFile(string fileName) {

		static GLuint unitSource = 0;
		GLuint unit = unitSource++;
		GLuint handle = 0;

		gli::texture2D tex(gli::load_dds(fileName.c_str()));

		if (tex.empty()) throw logic_error("failed to load texture " + fileName);

		auto levels = tex.levels();
		auto format = tex.format();

		if (gli::is_compressed(format)) throw logic_error("compressed textures not supported");

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(tex.levels()-1));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);

		glTexStorage2D(GL_TEXTURE_2D, GLint(tex.levels()),
				(GLenum)gli::internal_format(format),
				(GLsizei)tex.dimensions().x,
				(GLsizei)tex.dimensions().y);

		for (gli::texture2D::size_type lvl = 0; lvl < levels; ++lvl) {
			const gli::image& img = tex[lvl];
			glTexSubImage2D(GL_TEXTURE_2D, (GLint)lvl, 0, 0,
					(GLsizei)img.dimensions().x,
					(GLsizei)img.dimensions().y,
					(GLenum)gli::external_format(format),
					(GLenum)gli::type_format(format),
					img.data());
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return shared_ptr<Texture>(new Texture(handle, unit));
	}
};

}

#endif /* TEXTURE_HPP_ */
