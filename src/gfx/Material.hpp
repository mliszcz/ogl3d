/*
 * Material.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: michal
 */

#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "gli/gli.hpp"

#include "Texture.hpp"
#include "../Common.hpp"

namespace gfx {

class Material {

public:

	shared_ptr<glm::vec4> 	Ka;		// ambient
	shared_ptr<glm::vec4> 	Kd;		// diffuse
	shared_ptr<glm::vec4> 	Ks;		// specular
	shared_ptr<glm::vec4> 	Tr;		// transmittance
	shared_ptr<glm::vec4> 	Ke;		// emission
	shared_ptr<float> 		Ns;		// shininess (specular exponent), 0-1000
	shared_ptr<float>		d;		// dissolve
	shared_ptr<float>		Ni;		// ior (reflection index)

	shared_ptr<Texture>		mapKa;
	shared_ptr<Texture>		mapKd;
	shared_ptr<Texture>		mapKs;
};

}

#endif /* MATERIAL_HPP_ */
