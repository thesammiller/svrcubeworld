#include <GLFW/glfw3.h>
#include "Egl.h"
#include "VrApi.h"

//For the ovrTextureSwapChain
//#include "VrApi_Types.h"

typedef struct svrFramebuffer_s {
    int Width;
    int Height;
    int Multisamples;
    int TextureSwapChainLength;
    int TextureSwapChainIndex;
    bool UseMultiview;
    ovrTextureSwapChain* ColorTextureSwapChain;
    GLuint* DepthBuffers;
    GLuint* FrameBuffers;
} svrFramebuffer;

void svrFramebuffer_Clear(svrFramebuffer* frameBuffer);
bool svrFramebuffer_Create(
    svrFramebuffer* frameBuffer,
    const bool useMultiview,
    const GLenum colorFormat,
    const int width,
    const int height,
    const int multisamples);
void svrFramebuffer_Destroy(svrFramebuffer* frameBuffer);
void svrFramebuffer_SetCurrent(svrFramebuffer* frameBuffer);
void svrFramebuffer_SetNone();
void svrFramebuffer_Resolve(svrFramebuffer* frameBuffer);
void svrFramebuffer_Advance(svrFramebuffer* frameBuffer);
