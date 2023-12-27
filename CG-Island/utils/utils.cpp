#include "utils.h"

namespace mgl {
	float clamp(float min, float max, float value) {
		return std::max(min, std::min(max, value));
	}

	glm::mat4 interpolate_mat4(glm::mat4 initial_matrix, glm::mat4 target, float progress) {
		progress = glm::clamp(progress, 0.0f, 1.0f);

		// Interpolate each component of the matrices
		glm::mat4 interpolated_matrix;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				interpolated_matrix[i][j] = initial_matrix[i][j] + progress * (target[i][j] - initial_matrix[i][j]);
			}
		}

		return interpolated_matrix;
	}

	void printCurrentDir() {
		char cCurrentPath[FILENAME_MAX];

		if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		{
			printf("Error getting current directory\n");
		}

		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

		printf("The current working directory is %s\n", cCurrentPath);
	}
}
