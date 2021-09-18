#include "SvrCubeWorld.h"
//#include "Appl.h"


SvrCubeWorld::SvrCubeWorld()
    : svrAppl() //0, 0, CPU_LEVEL, GPU_LEVEL, true /* useMultiView */),
       {
    std::cout << "Created Server Cube World Object" << std::endl;    
    // VrApi_Helpers.h
    CenterEyeViewMatrix = ovrMatrix4f_CreateIdentity();
}

//SvrCubeWorld::SvrCubeWorld() {
//    
//}

bool SvrCubeWorld::AppInit() {
    std::cout << "SvrCubeWorld -- Initialize App" << std::endl;
    return true;
}