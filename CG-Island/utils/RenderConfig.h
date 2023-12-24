#pragma once
#include <functional>
#include <iostream>

#ifndef RENDER_CONFIG_H

struct RenderConfig {
    std::function<void()> sendUniforms = defaultSendUniforms;
    std::function<void()> setupTextures = defaultSetupTextures;

    static void defaultSendUniforms() {
        std::cout << "Default sendUniforms\n";
    }

    static void defaultSetupTextures() {
        std::cout << "Default setupTextures\n";
    }
};

#endif // !RENDER_CONFIG_H


