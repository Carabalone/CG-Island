#include <cassert>
#include <sstream>

#include "mglTexture.hpp"
#include "stb_image.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <cassert>

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
		std::cout << "uniform from updateShader:" << uniform << std::endl;
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

	////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
