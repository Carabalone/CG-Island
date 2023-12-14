#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../mgl/mgl.hpp"

class SceneNode {
public:
    mgl::Mesh* mesh;
    mgl::ShaderProgram* shader;
    glm::mat4 modelMatrix;

    SceneNode(mgl::Mesh* mesh, mgl::ShaderProgram* shader, const glm::mat4& modelMatrix = glm::mat4(1.0f))
        : mesh(mesh), shader(shader), modelMatrix(modelMatrix) {}

    void draw(GLuint ColorId, glm::vec4 color);
};

