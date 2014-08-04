/*
 * Camera.hpp
 *
 *  Created on: Aug 4, 2014
 *      Author: michal
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "../Common.hpp"

#include "glm/glm.hpp"

namespace gfx {

class Camera {

private:

	glm::vec3 cameraTarget;

public:

	glm::vec3 sphericalPosition; // (phi, theta, r)

//	glm::fquat g_orientation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);

public:

	Camera(const glm::vec3& position, const glm::vec3& target)
		: cameraTarget(target), sphericalPosition(position) { }

	const glm::vec3& target() {
		return cameraTarget;
	}

	glm::vec3 cameraPosition() {

		float r 	= sphericalPosition.z;
		float phi 	= util::DegToRad(sphericalPosition.x);
		float theta = util::DegToRad(sphericalPosition.y + 90.0f);	// theta=0 on X-Z plane

		float fSinTheta = sinf(theta);
		float fCosTheta = cosf(theta);
		float fCosPhi 	= cosf(phi);
		float fSinPhi 	= sinf(phi);

		/*
		 * spherical to cartesian
		 * 		x = r sinTh cosPhi
		 * 		y = r sinTh sinPhi
		 * 		z = r cosTh
		 * y and z are switched in OpenGL coordinate system
		 */
		glm::vec3 cartesianPosition = r * glm::vec3(fSinTheta*fCosPhi, fCosTheta, fSinTheta*fSinPhi);

		return cartesianPosition + cameraTarget;
	}

	glm::mat4 calculateLookAtMatrix() {

		return _calculateLookAtMatrix(cameraPosition(),
				cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	}

private:

	glm::mat4 _calculateLookAtMatrix(
			const glm::vec3& cameraPos,
			const glm::vec3& lookAtPos,
			const glm::vec3& upDirection) {

		glm::vec3 upDir = glm::normalize(upDirection);

		// new orthonormal camera coordinate system
		glm::vec3 lookDir 	= glm::normalize(lookAtPos-cameraPos);
		glm::vec3 rightDir 	= glm::normalize(glm::cross(lookDir, upDir));
		glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

		// rotation matrix into new coordinates
		glm::mat4 rotMat(1.0f);
		rotMat[0] = glm::vec4(rightDir, 0.0f);
		rotMat[1] = glm::vec4(perpUpDir, 0.0f);
		rotMat[2] = glm::vec4(-lookDir, 0.0f);

		// column-major order
		rotMat = glm::transpose(rotMat);

		glm::mat4 transMat(1.0f);
		transMat[3] = glm::vec4(-cameraPos, 1.0f);

		return rotMat * transMat;
	}
};

}

#endif /* CAMERA_HPP_ */
