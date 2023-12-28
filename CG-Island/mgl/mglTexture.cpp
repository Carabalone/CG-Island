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

	void Texture2D::generateNormalMap(float *pixels, int width, int height, float scale) {
		
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

	////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
