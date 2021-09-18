#include "SurfaceRender.h"


svrSurfaceRender::svrSurfaceRender() : CurrentSceneMatricesIdx(0) {}



void svrSurfaceRender::Init() {
    for (int i = 0; i < MAX_SCENEMATRICES_UBOS; i++) {
        SceneMatrices[i].Create(GLBUFFER_TYPE_UNIFORM, GlProgram::SCENE_MATRICES_UBO_SIZE, NULL);
    }

    CurrentSceneMatricesIdx = 0;
}
