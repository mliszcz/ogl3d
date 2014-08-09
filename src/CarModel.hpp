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

	// wheels

	const float wheelRotSpeed = util::DegToRad(40.0f);	// deg/s
	const float wheelMaxAngle = util::DegToRad(20.0f);	// deg
	const float wheelPerimeter = 4.0f;					// u
	float wheelAngle = util::DegToRad(0.0f);
	float wheelSpin  = util::DegToRad(0.0f);

	// position

	glm::vec3 carPosition = glm::vec3(0.0f,-0.25f, 0.0f); // model does not 'stand' on the ground (Y=0)

	const float carAcceleration = 10.0f;		// u/s2, u~2m
	const float carMaxSpeedForward = 15.0f;		// u/s
	const float carMaxSpeedBackward = 8.0f;		// u/s
	float carSpeed = 0.0f;
	float carAngle = 0.0f;

	// controls

	bool accelerating = false;
	bool decelerating = false;
	bool steeringLeft = false;
	bool steeringRight = false;

	//geometry

	const glm::vec3 wheelPosFL = glm::vec3(+2.53190f, +0.83931f, -1.49496f);
	const glm::vec3 wheelPosFR = glm::vec3(+2.53182f, +0.83931f, +1.49496f);

	const glm::vec3 wheelPosRL = glm::vec3(-2.21200f, +0.83929f, -1.50893f);
	const glm::vec3 wheelPosRR = glm::vec3(-2.21207f, +0.83930f, +1.50893f);

	const float frontAxle = wheelPosFL[0];
	const float rearAxle = -wheelPosRL[0];

public:

	CarModel(shared_ptr<gfx::Mesh> carMesh)
		: mesh(carMesh) { }

	const glm::vec3& position() {
		return carPosition;
	}

	void steerLeft(bool steer) {
		steeringLeft = steer;
		steeringRight = steeringRight && !steeringLeft;
	}

	void steerRight(bool steer) {
		steeringRight = steer;
		steeringLeft = steeringLeft && !steeringRight;
	}

	void accelerate(bool accel) {
		accelerating = accel;
		decelerating = decelerating && !accelerating;
	}

	void decelerate(bool decel) {
		decelerating = decel;
		accelerating = accelerating && !decelerating;
	}

	void update(float dt) {

		const float da = dt * wheelRotSpeed;
		const float dv = dt * carAcceleration;
		const float dr = dt * carSpeed;

		if (steeringLeft) wheelAngle += da;
		else if (steeringRight) wheelAngle -= da;
		else if (fabs(wheelAngle) > da) wheelAngle -= wheelAngle/fabs(wheelAngle) * da;
		else wheelAngle = 0.0f;

		wheelAngle = glm::clamp(wheelAngle, -wheelMaxAngle, wheelMaxAngle);

		wheelSpin -= 6.28 * carSpeed/wheelPerimeter * dt;

		if (accelerating) carSpeed += dv;
		else if (decelerating) carSpeed -= dv;
		else if (fabs(carSpeed) > dv) carSpeed -= carSpeed/fabs(carSpeed) * dv;
		else carSpeed = 0.0;

		carSpeed = glm::clamp(carSpeed, -carMaxSpeedBackward, carMaxSpeedForward);

		// locate axles
		// sin is negative, cause when +X goes right, +Z goes DOWN, and positive angle is measured counterclockwise

		glm::vec3 frontAxlePos = carPosition + frontAxle * glm::vec3(cosf(carAngle), 0, -sinf(carAngle));
		glm::vec3 rearAxlePos  = carPosition - rearAxle  * glm::vec3(cosf(carAngle), 0, -sinf(carAngle));

		frontAxlePos += dr * glm::vec3(cosf(carAngle+wheelAngle), 0, -sinf(carAngle+wheelAngle));
		rearAxlePos  += dr * glm::vec3(cosf(carAngle), 0, -sinf(carAngle));

		carPosition = (rearAxle*frontAxlePos + frontAxle*rearAxlePos)/(frontAxle+rearAxle);
		carAngle = glm::atan(-(frontAxlePos.z-rearAxlePos.z), frontAxlePos.x-rearAxlePos.x);

		printf("car speed: %f\n", carSpeed);
		printf("car angle: %f\n", util::RadToDeg(carAngle));
	}

	void draw(shared_ptr<shader::Program> program,
			shared_ptr<gfx::MatrixStack> modelToCameraStack) {

		with matrixStack(modelToCameraStack);

		modelToCameraStack->translate(carPosition);
		modelToCameraStack->rotateY(carAngle);

		for (const auto& sub : *mesh) {
			const string& name = sub.first;
			const gfx::Mesh::Component& comp = sub.second;

			with meshComponent(&comp);
			with matrixStack(modelToCameraStack);

			if (name.find("TireFL") != string::npos || name.find("BrakeFL") != string::npos) {

				modelToCameraStack->translate(wheelPosFL);
				modelToCameraStack->rotateY(wheelAngle);
				modelToCameraStack->rotateZ(wheelSpin);
				modelToCameraStack->translate(-wheelPosFL);

			} else if (name.find("TireFR") != string::npos || name.find("BrakeFR") != string::npos) {

				modelToCameraStack->translate(wheelPosFR);
				modelToCameraStack->rotateY(wheelAngle);
				modelToCameraStack->rotateZ(wheelSpin);
				modelToCameraStack->translate(-wheelPosFR);

			} else if (name.find("TireRL") != string::npos || name.find("BrakeRL") != string::npos) {

				modelToCameraStack->translate(wheelPosRL);
				modelToCameraStack->rotateZ(wheelSpin);
				modelToCameraStack->translate(-wheelPosRL);

			} else if (name.find("TireRR") != string::npos || name.find("BrakeRR") != string::npos) {

				modelToCameraStack->translate(wheelPosRR);
				modelToCameraStack->rotateZ(wheelSpin);
				modelToCameraStack->translate(-wheelPosRR);

			} else {
				// do not transform other parts
			}

			auto mat = comp.material();
			if (mat->Kd != nullptr) program->uniform("matKd") = *(mat->Kd);
			if (mat->Ks != nullptr) program->uniform("matKs") = *(mat->Ks);
			if (mat->Ns != nullptr) program->uniform("matNs") = *(mat->Ns);

			program->uniform("modelToCameraMatrix") = modelToCameraStack->top();
			program->uniform("cameraToModelMatrix") = glm::inverse(modelToCameraStack->top());

			comp.draw();
		}
	}
};


#endif /* CARMODEL_HPP_ */
