#pragma once

#include "../mgl/mgl.hpp"

class Callback {
public:
    virtual void beforeDraw() = 0;
    virtual void afterDraw() = 0;
    virtual ~Callback() = default;
};

class SilhouetteCallback : public Callback {
    void beforeDraw();
    void afterDraw();
};

