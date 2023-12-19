#include "ShaderManager.h"

namespace mgl {

    ShaderManager::ShaderManager() {
    }

    ShaderManager::~ShaderManager() {
    }

    void ShaderManager::addShader(std::string name, ShaderProgram* program) {
        shaders[name] = program;
    }

    ShaderProgram* ShaderManager::getShader(const std::string& name) {
        return shaders.at(name);
    }

    void ShaderManager::useShader(const std::string& name) {
        //auto it = shaders.find(name);
        //if (it != shaders.end()) {
        //    currentShader = name;
        //    it->second.bind();
        //}
        //else {
        //    std::cerr << "Shader not found: " << name << std::endl;
        //}
    }

    void ShaderManager::unbindCurrentShader() {
        glUseProgram(0);
    }

}  // namespace mgl

