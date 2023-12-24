#pragma once
#include <functional>
#include <iostream>

#ifndef RENDER_CONFIG_H

struct RenderConfig {
    std::function<void(mgl::ShaderProgram*)> sendUniforms = defaultSendUniforms;
    std::function<void()> setupTextures = defaultSetupTextures;

    static void defaultSendUniforms(mgl::ShaderProgram* program) {
    }

    static void defaultSetupTextures() {
    }
};

#endif // !RENDER_CONFIG_H


