#pragma once
#include <glm/glm.hpp>
#include <array>
#include "../mgl/mgl.hpp"

#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

class CameraManager
{
private:
	std::vector<mgl::Camera*> cameras;
	bool isOrtho;
	GLuint ubo;
public:
	mgl::Camera* getCurrentCamera();
	void switchCamera();
	void addCamera(mgl::Camera* camera);
	CameraManager();
	~CameraManager();
	void setOrtho(bool isOrtho);
	bool checkOrtho();
	CameraManager(GLuint bindingpoint);
	void sendMatrices();
	void updateCameraZoom(float scrollDelta);
};

#endif // CAMERA_MANAGER_H
