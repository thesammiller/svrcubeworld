#include "OVR_Math.h"
#include "VrApi_Helpers.h"

#pragma once

using OVR::Matrix4f;

#include "SurfaceRender.h"


enum svrRenderState {
        RENDER_STATE_LOADING, // show the loading icon
        RENDER_STATE_RUNNING, // render frames
        RENDER_STATE_ENDING, // show a black frame transition
    };

class svrAppl {
    public:
        svrAppl();
    
        virtual bool AppInit();

};

    /*
    protected:
        AppEyeGLStateSetup(const OVRFW::ovrApplFrameIn& in, const ovrFramebuffer* fb, int eye);
}
*/