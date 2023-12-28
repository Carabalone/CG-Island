#pragma once
#include <functional>
#include <iostream>
#include "../mgl/mgl.hpp"

#ifndef RENDER_CONFIG_H

struct RenderConfig {
    std::function<void(mgl::ShaderProgram*)> sendUniforms = defaultSendUniforms;
    std::function<void()> setupTextures = defaultSetupTextures;

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
};

#endif // !RENDER_CONFIG_H


