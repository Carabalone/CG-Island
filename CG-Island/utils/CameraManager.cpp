#include "CameraManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include "utils.h"

CameraManager::CameraManager(GLuint bindingpoint) : isOrtho(false)
{
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, 0, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingpoint, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

CameraManager::~CameraManager() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDeleteBuffers(1, &ubo);
}

bool CameraManager::checkOrtho() {
	return isOrtho;
}

void CameraManager::setOrtho(bool isOrtho) {
	this->isOrtho = isOrtho;
}

mgl::Camera* CameraManager::getCurrentCamera() {
	return isOrtho ? cameras[0] : cameras[1];
}

void CameraManager::switchCamera() {
	isOrtho = !isOrtho;

	mgl::Camera* currentCamera = getCurrentCamera();

	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(currentCamera->getViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(currentCamera->getProjectionMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CameraManager::addCamera(mgl::Camera* camera) {
	if (cameras.size() >= 2) {
		std::cout << "Cannot add another camera, max of 2 alread reached";
		return;
	}
	cameras.push_back(camera);
}

void CameraManager::sendMatrices() {
	if (cameras.empty()) {
		std::cerr << "Error: No cameras in the CameraManager." << std::endl;
		return;
	}

	// Get the current camera
	mgl::Camera* currentCamera = getCurrentCamera();
	if (!currentCamera) {
		std::cerr << "Error: No current camera set in the CameraManager." << std::endl;
		return;
	}

	currentCamera->setViewMatrix(currentCamera->calculateViewMatrix());

	// Update the view matrix in the UBO
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(currentCamera->getViewMatrix()));

	// Update the projection matrix in the UBO
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(currentCamera->getProjectionMatrix()));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void CameraManager::updateCameraZoom(float scrollDelta) {

	mgl::Camera* currentCamera = getCurrentCamera();

	float zoomFactor = scrollDelta < 0.0f ? 0.7f : 1.5f;
	glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(zoomFactor, zoomFactor, 1.0f));
	glm::mat4 zoomedOrthoProjection = currentCamera->getProjectionMatrix() * zoomMatrix;

	currentCamera->setProjectionMatrix(zoomedOrthoProjection);

}
