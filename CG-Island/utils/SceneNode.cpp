#include "sceneNode.h"
#include <glm/glm.hpp>

SceneNode::SceneNode(std::string name, glm::mat4 model, mgl::ShaderProgram* shader, mgl::Mesh* mesh, Callback* cb)
	: modelMatrix(model), shader(shader), mesh(mesh), parent(nullptr), callback(cb), worldMatrix(model), name(name) {
	textureNames = std::vector<std::string>();
	renderConfig = RenderConfig();
}

SceneNode::~SceneNode() {
	for (SceneNode* child : children) {
		delete child;
	}
}

void SceneNode::addChild(SceneNode* child) {
	children.push_back(child);
	child->parent = this;
}

void SceneNode::draw(mgl::ShaderProgram* optionalShader) {
	if (callback && useCallback) {
		callback->beforeDraw();
	}
	//GLuint ColorId = 1;
	glm::vec4 color(1.0f);
	glm::mat4 worldMatrix(1.0f);

	shader = shader ? shader : getParentShader();

	mgl::ShaderProgram* shaderToUse = optionalShader ? optionalShader : shader;

	glm::vec3 lightDirection = glm::vec3(1.0f);
	glm::vec3 lineColor = glm::vec3(0.0f);

	if (mesh && shaderToUse) {
		shaderToUse->bind();

			for (const auto& textureName : textureNames) {
				mgl::TextureInfo* textureInfo = mgl::TextureManager::getInstance().getTextureInfo(textureName);
				if (textureInfo) {
					textureInfo->updateShader(shaderToUse);
				}
			}

			updateModelMatrices();

			renderConfig.sendUniforms(shaderToUse);
			renderConfig.setupTextures();
			renderConfig.sendTime(shaderToUse);
			renderConfig.clipPlane(shaderToUse, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

			glUniformMatrix4fv(shaderToUse->Uniforms[mgl::MODEL_MATRIX].index, 1, GL_FALSE, glm::value_ptr(this->worldMatrix));
			//glUniform3fv(shaderToUse->Uniforms["lightDir"].index, 1, glm::value_ptr(lightDirection));
			//glUniform3fv(shaderToUse->Uniforms["lineColor"].index, 1, glm::value_ptr(lineColor));

			mesh->draw();

			for (const auto& textureName : textureNames) {
				mgl::TextureInfo* textureInfo = mgl::TextureManager::getInstance().getTextureInfo(textureName);
				if (textureInfo) {
					textureInfo->unbind();
				}
			}

		shaderToUse->unbind();
	}

	for (SceneNode* child : children) {
		child->draw();
	}

	if (callback && useCallback) {
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

SceneNode* SceneNode::getChild(std::string name) {
	for (SceneNode* child : children) {
		if (child->name == name) {
			return child;
		}
	}
	return nullptr;
}

void SceneNode::addTexture(std::string texname) {
	if (mgl::TextureManager::getInstance().exists(texname))
		textureNames.push_back(texname);
}
