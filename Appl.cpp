#include "Appl.h"
#include <iostream>


svrAppl::svrAppl() {
    std::cout << "Created Server Application Object" << std::endl;    
}

//Appl::Init()
/*
- This initializes the vrApi
- Creates the Context for GL
- Initializes the REnderr
- Framebuffers for each eye
*/


void svrAppl::AppRenderEye(
    const svrApplFrameIn& /* in */,
    svrRendererOutput& /* out */,
    int /* eye */) {}

bool svrAppl::AppInit() {
    std::cout << "Default AppInit called!" << std::endl;
    return false;
}

svrApplFrameOut svrAppl::AppFrame(const svrApplFrameIn& /* in */) {
    std::cout << "Default AppFrame called!" << std::endl;
    return svrApplFrameOut(false);
}


void svrAppl::DefaultRenderFrame_Loading(const svrApplFrameIn& in, svrRendererOutput& out) {
    NumLayers = 0;
    FrameFlags = 0u;
    // black layer
    ovrLayerProjection2 blackLayer = vrapi_DefaultLayerBlackProjection2();
    Layers[NumLayers++].Projection = blackLayer;
    // loading icon layer
    ovrLayerLoadingIcon2 iconLayer = vrapi_DefaultLayerLoadingIcon2();
    Layers[NumLayers++].LoadingIcon = iconLayer;
    FrameFlags |= VRAPI_FRAME_FLAG_FLUSH;
}

void svrAppl::DefaultRenderFrame_Running(const svrApplFrameIn& in, svrRendererOutput& out) {
    NumLayers = 0;
    FrameFlags = 0u;
    // set up layers
    int& layerCount = NumLayers;
    layerCount = 0;
    ovrLayerProjection2& layer = Layers[layerCount].Projection;
    layer = vrapi_DefaultLayerProjection2();
    //Would be great to have a default HeadPose to use
    layer.HeadPose = Tracking.HeadPose;
    //And now we're into the land of FrameBuffers
    for (int eye = 0; eye < VRAPI_FRAME_LAYER_EYE_MAX; ++eye) {
        svrFramebuffer* framebuffer = Framebuffer[NumFramebuffers == 1 ? 0 : eye].get();
        layer.Textures[eye].ColorSwapChain = framebuffer->ColorTextureSwapChain;
        layer.Textures[eye].SwapChainIndex = framebuffer->TextureSwapChainIndex;
        layer.Textures[eye].TexCoordsFromTanAngles = ovrMatrix4f_TanAngleMatrixFromProjection(
            (ovrMatrix4f*)&out.FrameMatrices.EyeProjection[eye]);
    }
    layer.Header.Flags |= VRAPI_FRAME_LAYER_FLAG_CHROMATIC_ABERRATION_CORRECTION;
    layerCount++;

    //TODO: HIJACK THIS SECTION --> SEND THE FRAMEBUFFER/RECEIVE THE FRAMEBUFFER
    // render images for each eye
    for (int eye = 0; eye < NumFramebuffers; ++eye) {
        svrFramebuffer* framebuffer = Framebuffer[eye].get();
        svrFramebuffer_SetCurrent(framebuffer);

        AppEyeGLStateSetup(in, framebuffer, eye);
        AppRenderEye(in, out, eye);

        svrFramebuffer_Resolve(framebuffer);
        svrFramebuffer_Advance(framebuffer);
    }

    ovrFramebuffer_SetNone();
}


void svrAppl::AppEyeGLStateSetup(
    const svrApplFrameIn& , //in
    const svrFramebuffer* fb,
    int ) { //eye
    GL(glEnable(GL_SCISSOR_TEST));
    GL(glDepthMask(GL_TRUE));
    GL(glEnable(GL_DEPTH_TEST));
    GL(glDepthFunc(GL_LEQUAL));
    GL(glEnable(GL_CULL_FACE));
    GL(glViewport(0, 0, fb->Width, fb->Height));
    GL(glScissor(0, 0, fb->Width, fb->Height));
    GL(glClearColor(0.016f, 0.0f, 0.016f, 1.0f));
    GL(glEnable(GL_FRAMEBUFFER_SRGB_EXT));
    GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL(glDisable(GL_FRAMEBUFFER_SRGB_EXT));
}

