#include "sceneNode.h"
#include <glm/glm.hpp>

SceneNode::SceneNode(glm::mat4 model, mgl::ShaderProgram* shader, mgl::Mesh* mesh, Callback* cb)
    : modelMatrix(model), shader(shader), mesh(mesh), parent(nullptr), callback(cb) {}

SceneNode::~SceneNode() {
    for (SceneNode* child : children) {
        delete child;
    }
}

void SceneNode::addChild(SceneNode* child) {
    children.push_back(child);
    child->parent = this;
}

void SceneNode::draw() {
    if (callback) {
        callback->beforeDraw();
    }
    //GLuint ColorId = 1;
    glm::vec4 color(1.0f);

    shader = shader ? shader : getParentShader();

		if (mesh && shader) {
			shader->bind();
			glUniformMatrix4fv(shader->Uniforms[mgl::MODEL_MATRIX].index, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));
			//glUniform4fv(ColorId, 1, glm::value_ptr(color));
			mesh->draw();
			shader->unbind();
		}

		for (SceneNode* child : children) {
			child->draw();
		}

    if (callback) {
        callback->afterDraw();
    }
}

mgl::ShaderProgram* SceneNode::getParentShader()
{
	SceneNode* parent = this->parent;
	while (parent != nullptr) {
		if (parent->shader != nullptr) {
			return parent->shader;
		}
		parent = parent->parent;
	}
	return nullptr;
}
