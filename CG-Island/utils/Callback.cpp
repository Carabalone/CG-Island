#include "Callback.h"

void SilhouetteCallback::beforeDraw() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
}

void SilhouetteCallback::afterDraw() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


void DepthTestCallback::beforeDraw() {
    glDepthMask(false);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DepthTestCallback::afterDraw() {
    glDepthMask(true);
    glDisable(GL_BLEND);
}
