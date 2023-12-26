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
    glDisable(GL_DEPTH_TEST);
}

void DepthTestCallback::afterDraw() {
	glEnable(GL_DEPTH_TEST);
}
