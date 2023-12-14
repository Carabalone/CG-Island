#pragma once

class Callback {
public:
    virtual void beforeDraw() = 0;
    virtual void afterDraw() = 0;
    virtual ~Callback() = default;
};

