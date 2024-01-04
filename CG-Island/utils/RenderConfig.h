#pragma once
#include <functional>
#include <iostream>
#include "../mgl/mgl.hpp"

#ifndef RENDER_CONFIG_H

struct RenderConfig {
    std::function<void(mgl::ShaderProgram*)> sendUniforms = defaultSendUniforms;
    std::function<void()> setupTextures = defaultSetupTextures;
    bool clip = false;

    static float time;

    static void defaultSendUniforms(mgl::ShaderProgram* program) {
    }

    static void defaultSetupTextures() {
    }

    void sendTime(mgl::ShaderProgram* shader) {
        if (shader->isUniform(mgl::TIME) && time >= 0.0f) {
			glUniform1f(shader->Uniforms[mgl::TIME].index, time);
        }
	}

    void clipPlane(mgl::ShaderProgram* shader, glm::vec4 plane) {
		glUniform1i(shader->Uniforms["clip"].index, clip);
        if (clip && shader->isUniform(mgl::CLIP_PLANE)) {
			glUniform4f(shader->Uniforms[mgl::CLIP_PLANE].index, plane.x, plane.y, plane.z, plane.w);
		}
	}
};

#endif // !RENDER_CONFIG_H


