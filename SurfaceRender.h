#include "GlGeometry.h"

#include "GpuState.h"
#include "GlProgram.h"
#include "GlBuffer.h"

#pragma once


#include <string>

struct svrSurfaceDef {
    svrSurfaceDef() : numInstances(1) {}

    // Name from the model file, can be used to control surfaces with code.
    // May be multiple semi-colon separated names if multiple source meshes
    // were merged into one surface.
    std::string surfaceName;

    // There is a space savings to be had with triangle strips
    // if primitive restart is supported, but it is a net speed
    // loss on most architectures.  Adreno docs still recommend,
    // so it might be worth trying.
    GlGeometry geo;

    // state + program + uniforms
    svrGraphicsCommand graphicsCommand;

    // Number of instances to be rendered  (0 or 1 denotes no instancing)
    int numInstances;
};

class svrSurfaceRender {
    public:
        svrSurfaceRender();

        //Requires an active GL context
         void Init();
    
    private:
        int CurrentSceneMatricesIdx;
        private:
        // Use a ring-buffer to avoid rendering hazards with potential update
        // of the SceneMatrices UBO multiple times per frame.
        static const int MAX_SCENEMATRICES_UBOS = 8;
        GlBuffer SceneMatrices[MAX_SCENEMATRICES_UBOS]; // ubo for storing per-view scene matrices which
                                                    // are common to the framework render programs
                                                    // and do not change frequently.

};


