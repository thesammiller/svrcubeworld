#include "OVR_Math.h"
#include "VrApi_Helpers.h"
#include "FrameParams.h"

#include <cstdint>
#include <vector>
#include <mutex>
#include <memory>

#pragma once

using OVR::Matrix4f;

#include "SurfaceRender.h"
#include "Framebuffer.h"


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
        void svrAppl::AppEyeGLStateSetup(const svrApplFrameIn& , const svrFramebuffer* fb, int );
        //Is this a no-op? There's no body to method?
        void AppRenderEye(const svrApplFrameIn& /* in */, svrRendererOutput& /* out */, int /* eye */);

    
    protected:
        //Used in Default Render Frame Loading & Running
        int NumLayers = 0;
        int FrameFlags = 0;
        //Used in AppRenderFrame
        float SuggestedEyeFovDegreesX = 90.0f;
        float SuggestedEyeFovDegreesY = 90.0f;
        ovrLayer_Union2 Layers[ovrMaxLayerCount] = {};
        int NumFramebuffers;
        std::unique_ptr<svrFramebuffer> Framebuffer[NUM_EYES];
        
        //Holds HeadPose, Projection Matrix, ViewMatrix
        ovrTracking2 Tracking;



};

    /*
    protected:
        AppEyeGLStateSetup(const OVRFW::ovrApplFrameIn& in, const ovrFramebuffer* fb, int eye);
}
*/