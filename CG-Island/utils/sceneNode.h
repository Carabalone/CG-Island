#pragma once
#include "Callback.h"
#include "../mgl/mgl.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "../mgl/mgl.hpp"
#include "TextureManager.h"
#include "RenderConfig.h"

class SceneNode {
private:
public:
    glm::mat4 modelMatrix;             // Pointer to a model matrix
    mgl::ShaderProgram* shader;         // Pointer to a shader program
    mgl::Mesh* mesh;                    // Pointer to a mesh
    std::vector<SceneNode*> children;   // Vector of children nodes
    Callback* callback;                 // Optional callback
    SceneNode* parent;                   // Pointer to the parent node
    glm::mat4 worldMatrix;
    std::string name;
    std::vector<std::string> textureNames;
    RenderConfig renderConfig;

public:
    // Constructor
    SceneNode(std::string name, glm::mat4 model, mgl::ShaderProgram* shader, mgl::Mesh* mesh, Callback* cb = nullptr);
    inline SceneNode() {
        modelMatrix = glm::mat4(1.0f);
        shader = nullptr;
        mesh = nullptr;
        callback = nullptr;
        parent = nullptr;
        renderConfig = RenderConfig();
    }

    // Destructor
    ~SceneNode();

    // Add a child node
    void addChild(SceneNode* child);

    // Draw method
    void draw();
    mgl::ShaderProgram* getParentShader();
    void updateModelMatrices();
    SceneNode* getChild(std::string name);
    void addTexture(std::string texname);
};

