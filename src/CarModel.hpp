/*
 * CarModel.hpp
 *
 *  Created on: Aug 6, 2014
 *      Author: michal
 */

#ifndef CARMODEL_HPP_
#define CARMODEL_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glm/glm.hpp"

#include "gfx/Mesh.hpp"
#include "gfx/MatrixStack.hpp"
#include "shader/Program.hpp"
#include "Common.hpp"

class CarModel {

private:

	shared_ptr<gfx::Mesh> mesh = nullptr;

	glm::fquat orientation = glm::fquat(1.0, 0.0, 0.0, 0.0);

	float wheelAngle = 0.0f;
	const glm::vec3 wheelPosFL = glm::vec3(-2.5f, 0.0f, +1.5f);
	const glm::vec3 wheelPosFR = glm::vec3(-2.5f, 0.0f, -1.5f);

public:

	CarModel(shared_ptr<gfx::Mesh> carMesh)
		: mesh(carMesh) { }

	void steerLeft() {
		wheelAngle += 10.0f;
		wheelAngle = glm::clamp(wheelAngle, -30.0f, 30.0f);
	}

	void steerRight() {
		wheelAngle -= 10.0f;
		wheelAngle = glm::clamp(wheelAngle, -30.0f, 30.0f);
	}

	void draw(shared_ptr<shader::Program> program,
			shared_ptr<gfx::MatrixStack> modelToCameraStack) {

		modelToCameraStack->push();
		modelToCameraStack->translate(0.0f, -1.0f, 0.0f);

		for (const auto& sub : *mesh) {
			const string& name = sub.first;
			const gfx::Mesh::Component& comp = sub.second;

			comp.bindVAO();

			modelToCameraStack->push();

			if (name.find("TireFL") != string::npos || name.find("BrakeFL") != string::npos) {
				modelToCameraStack->translate(-1.0f*wheelPosFL);
				modelToCameraStack->apply(glm::mat4_cast(
						orientation*OrientationQuat(glm::vec3(0.0f, 1.0f, 0.0f), wheelAngle)));
				modelToCameraStack->translate(+1.0f*wheelPosFL);
			} else if (name.find("TireFR") != string::npos || name.find("BrakeFR") != string::npos) {
				modelToCameraStack->translate(-1.0f*wheelPosFR);
				modelToCameraStack->apply(glm::mat4_cast(
						orientation*OrientationQuat(glm::vec3(0.0f, 1.0f, 0.0f), wheelAngle)));
				modelToCameraStack->translate(+1.0f*wheelPosFR);
			} else {
				modelToCameraStack->apply(glm::mat4_cast(orientation));
			}

			program->uniform("modelToCameraMatrix") = modelToCameraStack->top();

			comp.draw(program);

			modelToCameraStack->pop();


			comp.unbindVAO();
		}

		modelToCameraStack->pop();
	}

private:

	glm::fquat OrientationQuat(const glm::vec3 &_axis, float fAngDeg) {

		float fAngRad = util::DegToRad(fAngDeg);

		glm::vec3 axis = glm::normalize(_axis);

		axis = axis * sinf(fAngRad / 2.0f);
		float scalar = cosf(fAngRad / 2.0f);

		return glm::fquat(scalar, axis.x, axis.y, axis.z);
	}
};


#endif /* CARMODEL_HPP_ */
