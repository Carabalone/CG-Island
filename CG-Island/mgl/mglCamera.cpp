////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// Copyright (c)2023 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#include "./mglCamera.hpp"

#include <glm/gtc/type_ptr.hpp>
#include "glm/common.hpp"
#include "iostream"
#include "iomanip"
#include <glm/gtx/euler_angles.hpp>

void print_mat42(const glm::mat4& matrix, std::string matrixName) {
	const float* matData = glm::value_ptr(matrix);

	std::cout << matrixName << ":" << std::endl;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << std::setw(12) << std::setprecision(6) << std::fixed << matData[i + j * 4] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

namespace mgl {

	///////////////////////////////////////////////////////////////////////// Camera

	glm::mat4 Camera::calculateRotationMatrix()
	{
		rot += deltaRot;
		deltaRot = glm::vec2(0.0f);

		glm::vec3 x = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 z = glm::vec3(0.0f, 0.0f, 1.0f);

		glm::quat init(1.0f, 0.0f, 0.0f, 0.0f);

		// the code below does not have the axis inverted.
		// if you move the mouse up you expect the x axis to move.
		// check https://prnt.sc/yFMeuXaxAJ50 for visualization.
		glm::quat rotX = glm::angleAxis(glm::radians(rot.x), y);
		glm::quat rotY = glm::angleAxis(glm::radians(rot.y), x);

		glm::quat rot = rotY * rotX * init;

		return glm::mat4_cast(rot);
	}

	glm::mat4 Camera::calculateViewMatrix() {
		glm::mat4 translateRadius = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -radius));
		glm::mat4 rotate = calculateRotationMatrix();

		return translateRadius * rotate;
	}

	void Camera::updateOrientation(float deltaX, float deltaY) {
		deltaRot += glm::vec2(deltaX, deltaY);
	}

	Camera::Camera(GLuint bindingpoint) :
		ViewMatrix(glm::mat4(1.0f)), ProjectionMatrix(glm::mat4(1.0f)), rot(0.0f, 0.0f),
		center(glm::vec3(0.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), radius(5.0f), deltaRot(glm::vec2(0.0f))
	{
		//EMPTY
	}

	Camera::Camera(GLuint bindingpoint, glm::vec3 center, glm::vec3 up, float radius)
		: ViewMatrix(glm::mat4(1.0f)), ProjectionMatrix(glm::mat4(1.0f)), rot(0.0f, 0.0f),
		  deltaRot(glm::vec2(0.0f)) {
		this->center = center;
		this->up = up;
		this->radius = radius;
	}

	Camera::~Camera() {
		// EMPTY
	}

	glm::mat4 Camera::getViewMatrix() { return ViewMatrix; }

	void Camera::setViewMatrix(const glm::mat4& viewmatrix) {
		ViewMatrix = viewmatrix;
	}

	glm::mat4 Camera::getProjectionMatrix() { return ProjectionMatrix; }

	void Camera::setProjectionMatrix(const glm::mat4& projectionmatrix) {
		ProjectionMatrix = projectionmatrix;
	}

	void Camera::setRadius(float newRadius) {
		radius = std::max(MIN_RADIUS, std::min(MAX_RADIUS, newRadius));
	}

	float Camera::getRadius() { return radius; }

}  // namespace mgl
