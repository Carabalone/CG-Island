#include "sceneNode.h"
#include <glm/glm.hpp>

SceneNode::SceneNode(glm::mat4 model, mgl::ShaderProgram* shader, mgl::Mesh* mesh, Callback* cb)
	: modelMatrix(model), shader(shader), mesh(mesh), parent(nullptr), callback(cb), worldMatrix(model) {}

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
	glm::mat4 worldMatrix(1.0f);

	shader = shader ? shader : getParentShader();
	glm::vec3 lightDirection = glm::vec3(1.0f);
	glm::vec3 lineColor = glm::vec3(0.0f);

	if (mesh && shader) {
		shader->bind();
		updateModelMatrices();
		glUniformMatrix4fv(shader->Uniforms[mgl::MODEL_MATRIX].index, 1, GL_FALSE, glm::value_ptr(this->worldMatrix));
		glUniform3fv(shader->Uniforms["lightDir"].index, 1, glm::value_ptr(lightDirection));
		glUniform3fv(shader->Uniforms["lineColor"].index, 1, glm::value_ptr(lineColor));
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

void SceneNode::updateModelMatrices() {
	if (parent != nullptr) {
		worldMatrix = parent->worldMatrix * modelMatrix;
	}
	else {
		worldMatrix = modelMatrix;
	}

	for (SceneNode* child : children) {
		child->updateModelMatrices();
	}
}

