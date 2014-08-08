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

	glm::fquat carOrientation = glm::fquat(1.0, 0.0, 0.0, 0.0);
	glm::fquat wheelOrientation = glm::fquat(1.0, 0.0, 0.0, 0.0);

	float carAngle = 0.0f;

	const float wheelRotSpeed = 40.0f;	// deg/s
	const float wheelMaxAngle = 20.0f;	// deg
	float wheelAngle = 0.0f;
	float wheelSpin = 0.0f;

	glm::vec3 vecPosition = glm::vec3(0.0f,-0.25f, 0.0f); // model does not 'stand' on the ground (Y=0)
	glm::vec3 vecVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 vecAccelert = glm::vec3(0.0f, 0.0f, 0.0f);

	bool accelerating = false;
	bool decelerating = false;
//	float accelerationFactor = 0.0f;	// no unit, [-1, 1]

	float speedMax = 2.0f;	// u/s

	bool steeringLeft = false;
	bool steeringRight = false;

	const glm::vec3 wheelPosFL = glm::vec3(+2.53190f, +0.83931f, -1.49496f);
	const glm::vec3 wheelPosFR = glm::vec3(+2.53182f, +0.83931f, +1.49496f);

	const glm::vec3 wheelPosRL = glm::vec3(-2.21200f, +0.83929f, -1.50893f);
	const glm::vec3 wheelPosRR = glm::vec3(-2.21207f, +0.83930f, +1.50893f);

public:

	CarModel(shared_ptr<gfx::Mesh> carMesh)
		: mesh(carMesh) { }

	const glm::vec3& position() {
		return vecPosition;
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

	void recalc(float dt) {

		float da = dt * wheelRotSpeed;

		if (steeringLeft) wheelAngle += da;
		else if (steeringRight) wheelAngle -= da;
		else if(fabs(wheelAngle) > da)
			wheelAngle -= wheelAngle/fabs(wheelAngle) * da;

		wheelAngle = glm::clamp(wheelAngle, -wheelMaxAngle, wheelMaxAngle);

		wheelOrientation = OrientationQuat(glm::vec3(0.0f, 1.0f, 0.0f), wheelAngle);

		float wheelAngleRad = util::DegToRad(wheelAngle);

//		if (accelerating) vecAccelert = glm::vec3(cosf(-wheenAngleRad), 0.0f, sinf(-wheenAngleRad));
//		else if (decelerating) vecAccelert = -1.0f*glm::vec3(cosf(+wheenAngleRad), 0.0f, sinf(+wheenAngleRad));
//		else {
//			vecAccelert -= 0.5f * vecVelocity; //glm::length(vecVelocity) * glm::vec3(cosf(-wheenAngleRad), 0.0f, sinf(-wheenAngleRad));
//		}

		if (accelerating) wheelSpin -= 0.1f;
		else if (decelerating) wheelSpin += 0.1f;

		if (accelerating) speedMax = +1.0f;
		else if (decelerating) speedMax = -1.0f;
		else {
			speedMax = 0.0f;
		}

		speedMax *= 10.0f;

		float carAngleRad = util::DegToRad(-carAngle);

		float wFront = wheelPosFL[0];
		float wRear = -wheelPosRL[0];

		glm::vec2 pos = glm::vec2(vecPosition.x, vecPosition.z);
		glm::vec2 frontWheel = pos + wFront * glm::vec2( cosf(carAngleRad), -sinf(carAngleRad) );
		glm::vec2 backWheel = pos - wRear * glm::vec2( cosf(carAngleRad), -sinf(carAngleRad) );


		backWheel += speedMax * dt * glm::vec2(cosf(carAngleRad) , -sinf(carAngleRad));
		frontWheel += speedMax * dt * glm::vec2(cosf(carAngleRad+wheelAngleRad) , -sinf(carAngleRad+wheelAngleRad));

		glm::vec2 newPos = glm::vec2(wRear*frontWheel + wFront*backWheel) / (wFront+wRear);
		carAngleRad = glm::atan( frontWheel.y - backWheel.y, frontWheel.x - backWheel.x );

		vecPosition.x = newPos.x;
		vecPosition.z = newPos.y;

		carAngle = util::RadToDeg(carAngleRad);

//		vecAccelert *= 1.0f;

//		float speedCurr = glm::length(vecVelocity);
//		if (speedCurr < speedMax) {
//		    vecVelocity += vecAccelert * dt;
//		}

//		vecPosition += vecVelocity * dt;

//		float ang = -1.0f * util::RadToDeg(glm::atan(vecVelocity.z/vecVelocity.x));
//		printf("ang %f\n", ang);

		carOrientation = OrientationQuat(glm::vec3(0.0f, 1.0f, 0.0f), -carAngle);

//		printf("front (%f, %f)\trear (%f, %f)\n", frontWheel.x, frontWheel.y, backWheel.x, backWheel.y);
//		printf("car %f\twheel %f\n", carAngle, wheelAngle);

//		float ang = -1.0f * util::RadToDeg(glm::atan(vecVelocity.z/vecVelocity.x));

//		printf("angle: atan(%f/%f) =  %f\n", vecVelocity.x, vecVelocity.z, ang);
//		carOrientation = OrientationQuat(glm::vec3(0.0f, 1.0f, 0.0f), ang);
	}

	void draw(shared_ptr<shader::Program> program,
			shared_ptr<gfx::MatrixStack> modelToCameraStack) {

		with matrixStack(modelToCameraStack);

		modelToCameraStack->translate(vecPosition);

		for (const auto& sub : *mesh) {
			const string& name = sub.first;
			const gfx::Mesh::Component& comp = sub.second;

			with meshComponent(&comp);
			with matrixStack(modelToCameraStack);

			if (name.find("TireFL") != string::npos || name.find("BrakeFL") != string::npos) {

				modelToCameraStack->apply(glm::mat4_cast(carOrientation));
				modelToCameraStack->translate(+1.0f*wheelPosFL);
				modelToCameraStack->apply(glm::mat4_cast(wheelOrientation));
				modelToCameraStack->rotateZ(wheelSpin);
				modelToCameraStack->translate(-1.0f*wheelPosFL);

			} else if (name.find("TireFR") != string::npos || name.find("BrakeFR") != string::npos) {

				modelToCameraStack->apply(glm::mat4_cast(carOrientation));
				modelToCameraStack->translate(+1.0f*wheelPosFR);
				modelToCameraStack->apply(glm::mat4_cast(wheelOrientation));
				modelToCameraStack->rotateZ(wheelSpin);
				modelToCameraStack->translate(-1.0f*wheelPosFR);

			} else if (name.find("TireRL") != string::npos || name.find("BrakeRL") != string::npos) {

							modelToCameraStack->apply(glm::mat4_cast(carOrientation));
							modelToCameraStack->translate(+1.0f*wheelPosRL);
							modelToCameraStack->rotateZ(wheelSpin);
							modelToCameraStack->translate(-1.0f*wheelPosRL);

			} else if (name.find("TireRR") != string::npos || name.find("BrakeRR") != string::npos) {

							modelToCameraStack->apply(glm::mat4_cast(carOrientation));
							modelToCameraStack->translate(+1.0f*wheelPosRR);
							modelToCameraStack->rotateZ(wheelSpin);
							modelToCameraStack->translate(-1.0f*wheelPosRR);

			}else {
				modelToCameraStack->apply(glm::mat4_cast(carOrientation));
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
