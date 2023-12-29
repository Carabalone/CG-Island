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
		float* pixels = nullptr;
		bool storePixels = false;
		void bind() override;
		void unbind() override;
		void load(const std::string& filename);
		void loadFromPixelArray(float* image, int width, int height);
		void generatePerlinNoise(int width, int height, unsigned int seed);
		void generateNormalMap(float* pixels, int width, int height, float scale);
		void saveImageAsPPM(const std::string& filename, const float* pixels, int width, int height);
		void write_ppm(std::ostream& output, const float* pixels, int width, int height);
	};

	////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////// QUAD

	class Quad2D {
	private:
		GLuint _vaoId, _vboId;
		GLfloat _vertices[16];
		void destroy();

	public:
		static const GLuint POSITION = 0;
		static const GLuint TEXCOORD = 1;

		Quad2D();
		~Quad2D();
		void create();
		void draw();
	};

	////////////////////////////////////////////////////////////////// RENDER TARGET

	class RenderTargetTexture : Texture {
	private:
		Quad2D _quad;
		GLuint _framebuffer, _rboDepthStencil;
		GLfloat _r, _g, _b, _a;
		void destroy();
		void createColorTexture(const int width, const int height);
		void createRenderbufferObject(const int width, const int height);

	public:
		RenderTargetTexture();
		~RenderTargetTexture();
		void bind() override;
		void unbind() override;
		void create(const int width, const int height);
		void setFramebufferClearColor(const GLfloat r, const GLfloat g,
			const GLfloat b, const GLfloat a);
		void bindFramebuffer();
		void unbindFramebuffer();
		void renderQuad(ShaderProgram* program, std::string textureUniform);
	};

	class DepthTexture : Texture {
	private:
		GLuint _framebuffer;
		Quad2D _quad;
		//void destroy();
		void createDepthTexture(const int width, const int height);
	public :
		DepthTexture();
	//	~DepthTexture();
		void bind() override;
		void unbind() override;
		void create(const int width, const int height);
		void bindFramebuffer();
		void unbindFramebuffer();
		void renderQuad(ShaderProgram* program, std::string textureUniform);
	};

	enum class TEXTURE_ACTIVE_UNITS {

	};


	////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_TEXTURE_HPP */
