#include "sceneNode.h"
#include <glm/glm.hpp>

void SceneNode::draw(GLuint ColorId, glm::vec4 color) {
    if (mesh && shader) {
        shader->bind();
        glUniformMatrix4fv(shader->Uniforms[mgl::MODEL_MATRIX].index, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform4fv(ColorId, 1, glm::value_ptr(color));
        mesh->draw();
        shader->unbind();
    }
}