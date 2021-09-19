
#include "VrApi_Config.h"
#include "VrApi_Version.h"
#include "VrApi_Types.h"

OVR_VRAPI_EXPORT ovrTextureSwapChain* vrapi_CreateTextureSwapChain3(
    ovrTextureType type,
    int64_t format,
    int width,
    int height,
    int levels,
    int bufferCount);



/// Returns the number of textures in the swap chain.
OVR_VRAPI_EXPORT int vrapi_GetTextureSwapChainLength(ovrTextureSwapChain* chain);

/// Get the OpenGL name of the texture at the given index.
OVR_VRAPI_EXPORT unsigned int vrapi_GetTextureSwapChainHandle(
    ovrTextureSwapChain* chain,
    int index);

