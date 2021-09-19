#include "OVR_Math.h"
#include "VrApi_Helpers.h"
#include "FrameParams.h"

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

        //Creates the OpenGL Context
        virtual bool AppInit();
        //Handles the controller information
        virtual svrApplFrameOut AppFrame(const svrApplFrameIn& /* in */);
        //Render method
        virtual void AppRenderFrame(const svrApplFrameIn &in, svrRendererOutput& out);
        // Called in the AppRenderFrame if nothing's going on yet
        void DefaultRenderFrame_Loading(const svrApplFrameIn& in, svrRendererOutput& out);
        //Called when we actually have a surface to render
        void DefaultRenderFrame_Running(const svrApplFrameIn& in, svrRendererOutput& out);

    
    protected:
        //Used in Default Render Frame Loading & Running
        int NumLayers = 0;
        int FrameFlags = 0;
        //Used in AppRenderFrame
        float SuggestedEyeFovDegreesX = 90.0f;
        float SuggestedEyeFovDegreesY = 90.0f;
            ovrLayer_Union2 Layers[ovrMaxLayerCount] = {};


};

    /*
    protected:
        AppEyeGLStateSetup(const OVRFW::ovrApplFrameIn& in, const ovrFramebuffer* fb, int eye);
}
*/