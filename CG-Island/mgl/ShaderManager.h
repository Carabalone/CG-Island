#ifndef MGL_SHADER_MANAGER_H
#define MGL_SHADER_MANAGER_H

#include "mglShader.hpp"
#include <string>
#include <map>

namespace mgl {

    class ShaderManager {
    public:
        ShaderManager();
        ~ShaderManager();

        void loadShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile);

        // Access ShaderProgram instances
        ShaderProgram& getShader(const std::string& name);

        // Use a shader program
        void useShader(const std::string& name);

        void unbindCurrentShader();

    private:
        std::map<std::string, ShaderProgram> shaders;
        std::string currentShader; // Track the currently active shader
    };

}  // namespace mgl

#endif /* MGL_SHADER_MANAGER_HPP */
