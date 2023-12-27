////////////////////////////////////////////////////////////////////////////////
//
// Textures (small example for 2D textures)
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_TEXTURE_HPP
#define MGL_TEXTURE_HPP

#include <iostream>
#include <string>

#include "mglSampler.hpp"
#include "mglShader.hpp"
#include "PerlinNoise.h"

namespace mgl {

	class Texture;
	class Texture2D;
	struct TextureInfo;

	//////////////////////////////////////////////////////////////////////// TEXTURE

	class Texture {
	protected:
		GLuint id;

	public:
		Texture();
		~Texture();
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

	//////////////////////////////////////////////////////////////////// TextureInfo

	struct TextureInfo {
		GLenum unit;                // GL_TEXTUREi
		GLuint index;               // sampler index in shader
		std::string uniform;        // uniform name in shader
		Texture* texture = nullptr; // Texture (engine object)
		Sampler* sampler = nullptr; // Sampler (engine object)

		TextureInfo(GLenum textureunit, GLuint index, const std::string& uniform,
			Texture* texture, Sampler* sampler);
		TextureInfo() : unit(GL_TEXTURE0), index(0), texture(nullptr), sampler(nullptr) {}
		void updateShader(ShaderProgram* shader);
		void unbind();
	};

	/////////////////////////////////////////////////////////////////////// TEXTURES

	class Texture2D : public Texture {
	public:
		void bind() override;
		void unbind() override;
		void load(const std::string& filename);
		void loadFromPixelArray(float* image, int width, int height);
		void generatePerlinNoise(int width, int height, unsigned int seed);
		void saveImageAsPPM(const std::string& filename, const float* pixels, int width, int height);
		void write_ppm(std::ostream& output, const float* pixels, int width, int height);
	};

	////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_TEXTURE_HPP */
