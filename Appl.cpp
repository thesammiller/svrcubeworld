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

bool svrAppl::AppInit() {
    std::cout << "Default AppInit called!" << std::endl;
    return false;
}

svrApplFrameOut svrAppl::AppFrame(const svrApplFrameIn& /* in */) {
    std::cout << "Default AppFrame called!" << std::endl;
    return svrApplFrameOut(false);
}



/*
void svrAppl::AppEyeGLStateSetup(
    const ovrApplFrameIn& , //in
    const ovrFramebuffer* fb,
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
*/
