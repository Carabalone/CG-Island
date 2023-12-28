#pragma once
#ifndef MGL_TEXTURE_MANAGER_H
#define MGL_TEXTURE_MANAGER_H

#include <unordered_map>
#include "../mgl/mgl.hpp"
#include <string>

namespace mgl {

    class TextureManager {
    private:
        std::unordered_map<std::string, TextureInfo> textureMap;
        TextureManager();

    public:
        static TextureManager& getInstance() {
            static TextureManager instance;
            return instance;
        }
        ~TextureManager();

        TextureManager(const TextureManager&) = delete;
        TextureManager(TextureManager&&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;

        Texture* getTexture(const std::string& name);

        // Add texture with a given name
        void addTexture(const std::string& name, GLenum textureUnit, GLuint index, const std::string& uniform,
            Texture* texture, Sampler* sampler);

        // Get texture info by name
        TextureInfo* getTextureInfo(const std::string& name);
        bool exists(std::string);
    };

} // namespace mgl

#endif /* MGL_TEXTURE_MANAGER_H */
