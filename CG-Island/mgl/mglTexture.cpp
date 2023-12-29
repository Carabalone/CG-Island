#include <cassert>
#include <sstream>

#include "mglTexture.hpp"
#include "stb_image.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <cassert>
#include <iomanip>
#include "../utils/utils.h"

namespace mgl {

	//////////////////////////////////////////////////////////////////// TextureInfo

	TextureInfo::TextureInfo(GLenum _unit, GLuint _index,
		const std::string& _uniform, Texture* _texture,
		Sampler* _sampler) {
		unit = _unit;
		uniform = _uniform;
		index = _index;
		texture = _texture;
		sampler = _sampler;
	}

	void TextureInfo::updateShader(ShaderProgram* shader) {
		glActiveTexture(unit);
		texture->bind();
		if (sampler)
			sampler->bind(index);
		glUniform1i(shader->Uniforms[uniform].index, index);
	}

	void TextureInfo::unbind() {
		texture->unbind();
		if (sampler)
			sampler->unbind(index);
	}

	//////////////////////////////////////////////////////////////////////// Texture

	Texture::Texture() : id(-1) {}

	Texture::~Texture() {}

	////////////////////////////////////////////////////////////////////// Texture2D

	void Texture2D::bind() { glBindTexture(GL_TEXTURE_2D, id); }

	void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	bool fileExists(const std::string& filename) {
		std::ifstream file(filename.c_str());
		return file.good();
	}

	void Texture2D::load(const std::string& filename) {

		if (!fileExists(filename)) {
			std::cout << "[DEBUG] File " << filename << " does not exist." << std::endl;
			exit(EXIT_FAILURE);
		}

		stbi_set_flip_vertically_on_load(true);
		int width, height, channels;
		std::cout << "Loading image file " << filename << "... ";
		unsigned char* image =
			stbi_load(filename.c_str(), &width, &height, &channels, 0);
		assert(channels == 4);
		if (image == nullptr) {
			std::cout << "error." << std::endl;
			exit(EXIT_FAILURE);
		}
		else {
			std::cout << "ok. " << std::endl;
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		// Pré OpenGL v3.30 (still compatible with core)

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		//                GL_LINEAR_MIPMAP_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, image);
		// syntax: glTexImage2D(target, level, internalformat, width, height, border,
		// format, type, data)

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(image);
	}

	void Texture2D::loadFromPixelArray(float* image, int width, int height) {

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		//                GL_LINEAR_MIPMAP_LINEAR);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
			GL_FLOAT, image);
		// syntax: glTexImage2D(target, level, internalformat, width, height, border,
		// format, type, data)

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void Texture2D::generatePerlinNoise(int width, int height, unsigned int seed) {
		printCurrentDir();

		PerlinNoise perlinNoise(seed);

		float* pixels = new float[width * height * 4];

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {

				float nx = static_cast<float>(x) / static_cast<float>(width);
				float ny = static_cast<float>(y) / static_cast<float>(height);

				float value = perlinNoise.noise(5.0f * nx, 5.0f * ny);

				int index = (y * width + x) * 4;
				pixels[index] = value;
				pixels[index + 1] = value;
				pixels[index + 2] = value;
				pixels[index + 3] = 1.0f; // Alpha channel
			}
		}

		loadFromPixelArray(pixels, width, height);

		this->pixels = pixels;
		storePixels = true;
	}

	void Texture2D::generateNormalMap(float* pixels, int width, int height, float scale) {

		float* normals = new float[width * height * 4];

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; x++) {

				float heightL = pixels[y * width + (x - 1 + width) % width];
				float heightR = pixels[y * width + (x + 1) % width];
				float heightD = pixels[((y - 1 + height) % height) * width + x];
				float heightU = pixels[((y + 1) % height) * width + x];

				glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));

				// Set the normals in the normals array
				int index = (y * width + x) * 4;
				normals[index] = normal.x;
				normals[index + 1] = normal.y;
				normals[index + 2] = normal.z;
				normals[index + 3] = 1.0f; // Alpha channel

			}
		}

		loadFromPixelArray(normals, width, height);

	}

	void Texture2D::saveImageAsPPM(const std::string& filename, const float* pixels, int width, int height) {
		std::ofstream outputFile(filename, std::ios::binary);

		if (outputFile.is_open()) {
			// Save the image in PPM format
			write_ppm(outputFile, pixels, width, height);

			outputFile.close();
		}
	}

	void Texture2D::write_ppm(std::ostream& output, const float* pixels, int width, int height) {
		// Write the PPM header
		output << "P6\n";
		output << width << " " << height << "\n";
		output << "255\n";

		// Convert float pixel values to unsigned char (0-255 range)
		unsigned char* pixelData = new unsigned char[width * height * 3];
		for (int i = 0; i < width * height * 3; ++i) {
			pixelData[i] = static_cast<unsigned char>(pixels[i] * 255.0f);
		}

		// Write the pixel data
		output.write(reinterpret_cast<const char*>(pixelData), width * height * 3);

		delete[] pixelData;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////// Quad2D

	GLfloat data[] = { 1.0f, -1.0f, 1.0f, 0.0f, // (x,y) (u,v)
					   1.0f,  1.0f, 1.0f, 1.0f,
					  -1.0f, -1.0f, 0.0f, 0.0f,
					  -1.0f,  1.0f, 0.0f, 1.0f };

	Quad2D::Quad2D() : _vaoId(-1), _vboId(-1) {

		memcpy(_vertices, data, sizeof(data));
	}

	Quad2D::~Quad2D() { destroy(); }

	void Quad2D::create() {

		glGenVertexArrays(1, &_vaoId);
		glGenBuffers(1, &_vboId);

		glBindVertexArray(_vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, _vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(POSITION);
		glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
			0);
		glEnableVertexAttribArray(TEXCOORD);
		glVertexAttribPointer(TEXCOORD, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
			(void*)(2 * sizeof(GLfloat)));
		glBindVertexArray(0);
	}

	void Quad2D::draw() {

		glBindVertexArray(_vaoId);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void Quad2D::destroy() {

		glDeleteBuffers(1, &_vboId);
		glDeleteVertexArrays(1, &_vaoId);
	}

	/////////////////////////////////////////////////////////////////// RenderTarget

	RenderTargetTexture::RenderTargetTexture()
		: _framebuffer(-1), _rboDepthStencil(-1), _r(0.0f), _g(0.0f), _b(0.0f),
		_a(1.0f) {}

	RenderTargetTexture::~RenderTargetTexture() { destroy(); }

	void RenderTargetTexture::bind() { glBindTexture(GL_TEXTURE_2D, id); }

	void RenderTargetTexture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	void RenderTargetTexture::createColorTexture(const int width,
		const int height) {
		// Create texture to hold color buffer
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
			GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			id, 0);
	}

	// glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum
	// textarget, GLuint texture, GLint level);
	// Attach a level of a texture object as a logical buffer of a framebuffer
	// object.
	//
	// target: specifies the framebuffer target. The symbolic constant must be
	// GL_FRAMEBUFFER.
	// attachment: specifies the attachment point to which an image from texture
	// should be attached. Must be one of: GL_COLOR_ATTACHMENT0,
	// GL_DEPTH_ATTACHMENT, or GL_STENCIL_ATTACHMENT.
	// textarget: specifies the texture target. Must be one of: GL_TEXTURE_2D,
	// GL_TEXTURE_CUBE_MAP_(POSITIVE|NEGATIVE)_(X|Y|Z).
	// texture: specifies the texture object whose image is to be attached.
	// level: specifies the mipmap level of the texture image to be attached, which
	// must be 0.

	void RenderTargetTexture::createRenderbufferObject(const int width,
		const int height) {
		// Create Renderbuffer object to hold depth and stencil buffers
		glGenRenderbuffers(1, &_rboDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, _rboDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, _rboDepthStencil);
	}

	// glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum
	// renderbuffertarget, GLuint renderbuffer);
	// Attach a renderbuffer object to a framebuffer object.
	//
	// target: specifies the framebuffer target. The symbolic constant must be
	// GL_FRAMEBUFFER.
	// attachment: specifies the attachment point to which renderbuffer should be
	// attached. Must be one of: GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, or
	// GL_STENCIL_ATTACHMENT.
	// renderbuffertarget: specifies the renderbuffer target. The symbolic constant
	// must be GL_RENDERBUFFER.
	// renderbuffer: Specifies the renderbuffer object that is to be attached.

	void RenderTargetTexture::create(const int width, const int height) {

		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		createColorTexture(width, height);
		createRenderbufferObject(width, height);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTargetTexture::setFramebufferClearColor(const GLfloat r,
		const GLfloat g,
		const GLfloat b,
		const GLfloat a) {
		_r = r;
		_g = g;
		_b = b;
		_a = a;
	}

	void RenderTargetTexture::bindFramebuffer() {

		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glClearColor(_r, _g, _b, _a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void RenderTargetTexture::unbindFramebuffer() {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTargetTexture::renderQuad(ShaderProgram* program,
		std::string textureUniform) {
		program->bind();
		glUniform1i(program->Uniforms[textureUniform].index, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);

		glDisable(GL_DEPTH_TEST);
		_quad.create();
		_quad.draw();
		glEnable(GL_DEPTH_TEST);

		glBindTexture(GL_TEXTURE_2D, 0);
		program->unbind();
	}

	void RenderTargetTexture::destroy() {

		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &id);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &_rboDepthStencil);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &_framebuffer);
	}

	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////// DepthTexture ////////////////////

	DepthTexture::DepthTexture() : _framebuffer(-1) {}

	//DepthTexture::~DepthTexture() { destroy(); }

	void DepthTexture::bind() { glBindTexture(GL_TEXTURE_2D, id); }

	void DepthTexture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	void DepthTexture::bindFramebuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glClear(GL_DEPTH_BUFFER_BIT); // we only use the depth buffer
	}

	void DepthTexture::unbindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void DepthTexture::create(int width, int height) {
		glGenFramebuffers(1, &_framebuffer);

		createDepthTexture(width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
			id, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DepthTexture::createDepthTexture(int width, int height) {

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
						width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST = no interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT = repeat texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	void DepthTexture::renderQuad(mgl::ShaderProgram* shader, std::string uniform) {
		shader->bind();
		{
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, id);

			glUniform1i(shader->Uniforms[uniform].index, 4);

			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

				_quad.create();
				_quad.draw();

			glEnable(GL_DEPTH_TEST);

			glBindTexture(GL_TEXTURE_2D, 0);

		}
		shader->unbind();
	}

	/*void DepthTexture::destroy() {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &_framebuffer);
	}*/

	//////////////////////////////////////////////////
} // namespace mgl
