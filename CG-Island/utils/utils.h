#pragma once
#include <utility>
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include "CameraManager.h"
#include "Callback.h"
#include "InputManager.h"
#include "sceneNode.h"
#include "RenderConfig.h"
#include "TextureManager.h"
#include "Material.h"

#ifndef UTILS_H
#define UTILS_H

namespace mgl {
	float clamp(float min, float max, float value);
	glm::mat4 interpolate_mat4(glm::mat4 initial_matrix, glm::mat4 target, float progress);
}

#endif // !UTILS_H
