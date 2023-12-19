#include "Callback.h"

void SilhouetteCallback::beforeDraw() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
}

void SilhouetteCallback::afterDraw() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

