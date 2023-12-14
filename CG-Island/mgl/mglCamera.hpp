////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// Copyright (c)2023 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace mgl {

	class Camera;

	///////////////////////////////////////////////////////////////////////// Camera

	class Camera {
	private:
		GLuint UboId;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		float radius;
		float MIN_RADIUS = 1.0f, MAX_RADIUS = 50.0f;

	public:
		glm::vec2 deltaRot;
		glm::vec2 rot;
		glm::vec3 center;
		glm::vec3 up;
		glm::vec3 pos;
		glm::mat4 calculateViewMatrix();
		void updateOrientation(float deltaX, float deltaY);
		glm::mat4 calculateRotationMatrix();
		explicit Camera(GLuint bindingpoint);
		Camera(GLuint bindingpoint, glm::vec3 center, glm::vec3 up, float radius);
		virtual ~Camera();
		glm::mat4 getViewMatrix();
		void setViewMatrix(const glm::mat4& viewmatrix);
		glm::mat4 getProjectionMatrix();
		void setProjectionMatrix(const glm::mat4& projectionmatrix);
		void setRadius(float newRadius);
		float getRadius();
	};

	////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_CAMERA_HPP */
