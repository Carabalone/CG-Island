#pragma once
#include "../Callback.h"
#include "../mgl/mgl.hpp"
#include <glm/gtc/type_ptr.hpp>

class SceneNode {
private:
    glm::mat4* modelMatrix;             // Pointer to a model matrix
    mgl::ShaderProgram* shader;         // Pointer to a shader program
    mgl::Mesh* mesh;                    // Pointer to a mesh
    std::vector<SceneNode*> children;   // Vector of children nodes
    SceneNode* parent;                   // Pointer to the parent node
    Callback* callback;                 // Optional callback

public:
    // Constructor
    SceneNode(glm::mat4* model, mgl::ShaderProgram* shader, mgl::Mesh* mesh, Callback* cb = nullptr);

    // Destructor
    ~SceneNode();

    // Add a child node
    void addChild(SceneNode* child);

    // Draw method
    void draw();
};

