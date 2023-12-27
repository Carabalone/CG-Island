#pragma once

#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <vector>

namespace mgl {

	class PerlinNoise {
	public:
		PerlinNoise(unsigned int seed);

		float noise(float x, float y);

	private:
		static const int gradientSize = 256;
		std::vector<int> permutation;

		static const int p[gradientSize * 2];

		static float fade(float t);

		static float lerp(float t, float a, float b);

		static float grad(int hash, float x, float y);
	};
}

#endif  // PERLIN_NOISE_H

