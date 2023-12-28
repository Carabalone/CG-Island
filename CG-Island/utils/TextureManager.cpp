#include "TextureManager.h"

namespace mgl {

    TextureManager::TextureManager() {
        textureMap = std::unordered_map<std::string, TextureInfo>();
    }

    TextureManager::~TextureManager() {
        for (auto& pair : textureMap) {
            delete pair.second.texture;
            delete pair.second.sampler;
        }
    }

    void TextureManager::addTexture(const std::string& name, GLenum textureUnit, GLuint index, const std::string& uniform,
        Texture* texture, Sampler* sampler) {
        textureMap[name] = TextureInfo(textureUnit, index, uniform, texture, sampler);
    }

    TextureInfo* TextureManager::getTextureInfo(const std::string& name) {
        auto it = textureMap.find(name);
        return (it != textureMap.end()) ? &(it->second) : nullptr;
    }

    bool TextureManager::exists(std::string texname) {
        return textureMap.find(texname) != textureMap.end();
    }

    Texture* TextureManager::getTexture(const std::string& name) {
		auto it = textureMap.find(name);
		return (it != textureMap.end()) ? it->second.texture : nullptr;
	}

} // namespace mgl