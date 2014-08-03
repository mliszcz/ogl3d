/*
 * Hierarchy.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: michal
 */

#ifndef HIERARCHY_HPP_
#define HIERARCHY_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glm/glm.hpp"

#include "Common.hpp"

#include "shader/Program.hpp"

#include "gfx/MatrixStack.hpp"
#include "gfx/Mesh.hpp"

class Hierarchy
{
public:
	Hierarchy(shared_ptr<shader::Program> _program, shared_ptr<gfx::Mesh> _mesh)
		: program(_program)
		, mesh(_mesh)
		, posBase(glm::vec3(3.0f, -5.0f, -40.0f))
		, angBase(-45.0f)
		, posBaseLeft(glm::vec3(2.0f, 0.0f, 0.0f))
		, posBaseRight(glm::vec3(-2.0f, 0.0f, 0.0f))
		, scaleBaseZ(3.0f)
		, angUpperArm(-33.75f)
		, sizeUpperArm(9.0f)
		, posLowerArm(glm::vec3(0.0f, 0.0f, 8.0f))
		, angLowerArm(146.25f)
		, lenLowerArm(5.0f)
		, widthLowerArm(1.5f)
		, posWrist(glm::vec3(0.0f, 0.0f, 5.0f))
		, angWristRoll(0.0f)
		, angWristPitch(67.5f)
		, lenWrist(2.0f)
		, widthWrist(2.0f)
		, posLeftFinger(glm::vec3(1.0f, 0.0f, 1.0f))
		, posRightFinger(glm::vec3(-1.0f, 0.0f, 1.0f))
		, angFingerOpen(180.0f)
		, lenFinger(2.0f)
		, widthFinger(0.5f)
		, angLowerFinger(45.0f)
	{}

	void Draw()
	{
		gfx::MatrixStack modelToCameraStack;

		program->use();
		mesh->bindVAO();

		modelToCameraStack.translate(posBase);
		modelToCameraStack.rotateY(angBase);

		//Draw left base.
		{
			modelToCameraStack.push();
			modelToCameraStack.translate(posBaseLeft);
			modelToCameraStack.scale(1.0f, 1.0f, scaleBaseZ);
//			modelToCameraStack.mul(gfx::Matrix<4, 4, float>::Translation(posBaseLeft));
//			modelToCameraStack.mul(gfx::Matrix<4, 4, float>::Scale(1.0f, 1.0f, scaleBaseZ));
			program->uniform("modelToCameraMatrix") = modelToCameraStack.top();
			mesh->draw();
			modelToCameraStack.pop();
		}

		//Draw right base.
		{
			modelToCameraStack.push();
			modelToCameraStack.translate(posBaseRight);
			modelToCameraStack.scale(1.0f, 1.0f, scaleBaseZ);
//			modelToCameraStack.mul(gfx::Matrix<4, 4, float>::Translation(posBaseRight));
//			modelToCameraStack.mul(gfx::Matrix<4, 4, float>::Scale(1.0f, 1.0f, scaleBaseZ));
			program->uniform("modelToCameraMatrix") = modelToCameraStack.top();
			mesh->draw();
			modelToCameraStack.pop();
		}

		//Draw main arm.
		DrawUpperArm(modelToCameraStack);

		mesh->unbindVAO();
		program->dispose();
	}

#define STANDARD_ANGLE_INCREMENT 11.25f
#define SMALL_ANGLE_INCREMENT 9.0f

	inline float Clamp(float fValue, float fMinValue, float fMaxValue)
	{
		if(fValue < fMinValue)
			return fMinValue;

		if(fValue > fMaxValue)
			return fMaxValue;

		return fValue;
	}

	void AdjBase(bool bIncrement)
	{
		angBase += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angBase = fmodf(angBase, 360.0f);
	}

	void AdjUpperArm(bool bIncrement)
	{
		angUpperArm += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angUpperArm = Clamp(angUpperArm, -90.0f, 0.0f);
	}

	void AdjLowerArm(bool bIncrement)
	{
		angLowerArm += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angLowerArm = Clamp(angLowerArm, 0.0f, 146.25f);
	}

	void AdjWristPitch(bool bIncrement)
	{
		angWristPitch += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angWristPitch = Clamp(angWristPitch, 0.0f, 90.0f);
	}

	void AdjWristRoll(bool bIncrement)
	{
		angWristRoll += bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
		angWristRoll = fmodf(angWristRoll, 360.0f);
	}

	void AdjFingerOpen(bool bIncrement)
	{
		angFingerOpen += bIncrement ? SMALL_ANGLE_INCREMENT : -SMALL_ANGLE_INCREMENT;
		angFingerOpen = Clamp(angFingerOpen, 9.0f, 180.0f);
	}

	void WritePose()
	{
		printf("angBase:\t%f\n", angBase);
		printf("angUpperArm:\t%f\n", angUpperArm);
		printf("angLowerArm:\t%f\n", angLowerArm);
		printf("angWristPitch:\t%f\n", angWristPitch);
		printf("angWristRoll:\t%f\n", angWristRoll);
		printf("angFingerOpen:\t%f\n", angFingerOpen);
		printf("\n");
	}

//private:
//	void DrawFingers(gfx::MatrixStack &modelToCameraStack)
//	{
//		//Draw left finger
//		modelToCameraStack.Push();
//		modelToCameraStack.Translate(posLeftFinger);
//		modelToCameraStack.RotateY(angFingerOpen);
//
//		modelToCameraStack.Push();
//		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
//		modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
//		glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
//		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
//		modelToCameraStack.Pop();
//
//		{
//			//Draw left lower finger
//			modelToCameraStack.Push();
//			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger));
//			modelToCameraStack.RotateY(-angLowerFinger);
//
//			modelToCameraStack.Push();
//			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
//			modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
//			glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
//			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
//			modelToCameraStack.Pop();
//
//			modelToCameraStack.Pop();
//		}
//
//		modelToCameraStack.Pop();
//
//		//Draw right finger
//		modelToCameraStack.Push();
//		modelToCameraStack.Translate(posRightFinger);
//		modelToCameraStack.RotateY(-angFingerOpen);
//
//		modelToCameraStack.Push();
//		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
//		modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
//		glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
//		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
//		modelToCameraStack.Pop();
//
//		{
//			//Draw right lower finger
//			modelToCameraStack.Push();
//			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger));
//			modelToCameraStack.RotateY(angLowerFinger);
//
//			modelToCameraStack.Push();
//			modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
//			modelToCameraStack.Scale(glm::vec3(widthFinger / 2.0f, widthFinger/ 2.0f, lenFinger / 2.0f));
//			glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
//			glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
//			modelToCameraStack.Pop();
//
//			modelToCameraStack.Pop();
//		}
//
//		modelToCameraStack.Pop();
//	}
//
//	void DrawWrist(MatrixStack &modelToCameraStack)
//	{
//		modelToCameraStack.Push();
//		modelToCameraStack.Translate(posWrist);
//		modelToCameraStack.RotateZ(angWristRoll);
//		modelToCameraStack.RotateX(angWristPitch);
//
//		modelToCameraStack.Push();
//		modelToCameraStack.Scale(glm::vec3(widthWrist / 2.0f, widthWrist/ 2.0f, lenWrist / 2.0f));
//		glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
//		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
//		modelToCameraStack.Pop();
//
//		DrawFingers(modelToCameraStack);
//
//		modelToCameraStack.Pop();
//	}
//
//	void DrawLowerArm(MatrixStack &modelToCameraStack)
//	{
//		modelToCameraStack.Push();
//		modelToCameraStack.Translate(posLowerArm);
//		modelToCameraStack.RotateX(angLowerArm);
//
//		modelToCameraStack.Push();
//		modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenLowerArm / 2.0f));
//		modelToCameraStack.Scale(glm::vec3(widthLowerArm / 2.0f, widthLowerArm / 2.0f, lenLowerArm / 2.0f));
//		glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(modelToCameraStack.Top()));
//		glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
//		modelToCameraStack.Pop();
//
//		DrawWrist(modelToCameraStack);
//
//		modelToCameraStack.Pop();
//	}
//
	void DrawUpperArm(gfx::MatrixStack &modelToCameraStack)
	{
		modelToCameraStack.push();
		modelToCameraStack.rotateX(angUpperArm);

		{
			modelToCameraStack.push();
			modelToCameraStack.translate(0.0f, 0.0f, (sizeUpperArm / 2.0f) - 1.0f);
			modelToCameraStack.scale(1.0f, 1.0f, sizeUpperArm / 2.0f);
//			modelToCameraStack.mul(gfx::Matrix<4, 4, float>::Translation(0.0f, 0.0f, (sizeUpperArm / 2.0f) - 1.0f));
//			modelToCameraStack.mul(gfx::Matrix<4, 4, float>::Scale(1.0f, 1.0f, sizeUpperArm / 2.0f));
			program->uniform("modelToCameraMatrix") = modelToCameraStack.top();
			mesh->draw();
			modelToCameraStack.pop();
		}

//		DrawLowerArm(modelToCameraStack);

		modelToCameraStack.pop();
	}

	shared_ptr<shader::Program> program;
	shared_ptr<gfx::Mesh> mesh;

	glm::vec3		posBase;
	float			angBase;

	glm::vec3		posBaseLeft, posBaseRight;
	float			scaleBaseZ;

	float			angUpperArm;
	float			sizeUpperArm;

	glm::vec3		posLowerArm;
	float			angLowerArm;
	float			lenLowerArm;
	float			widthLowerArm;

	glm::vec3		posWrist;
	float			angWristRoll;
	float			angWristPitch;
	float			lenWrist;
	float			widthWrist;

	glm::vec3		posLeftFinger, posRightFinger;
	float			angFingerOpen;
	float			lenFinger;
	float			widthFinger;
	float			angLowerFinger;
};

#endif /* HIERARCHY_HPP_ */
