#include "ShaderManager.h"

namespace mgl {

    ShaderManager::ShaderManager() {
    }

    ShaderManager::~ShaderManager() {
    }

    void ShaderManager::loadShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile) {
        ShaderProgram shaderProgram;
        shaderProgram.addShader(GL_VERTEX_SHADER, vertexFile);
        shaderProgram.addShader(GL_FRAGMENT_SHADER, fragmentFile);
        shaderProgram.create();
        shaders[name] = shaderProgram;
    }

    ShaderProgram& ShaderManager::getShader(const std::string& name) {
        return shaders.at(name);
    }

    void ShaderManager::useShader(const std::string& name) {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            currentShader = name;
            it->second.bind();
        }
        else {
            std::cerr << "Shader not found: " << name << std::endl;
        }
    }

    void ShaderManager::unbindCurrentShader() {
        glUseProgram(0);
    }

}  // namespace mgl

