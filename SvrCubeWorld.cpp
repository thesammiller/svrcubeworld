#include "SvrCubeWorld.h"
//#include "Appl.h"

SvrCubeWorld::SvrCubeWorld() {
    std::cout << "Created Server Cube World Object" << std::endl;    
}

bool SvrCubeWorld::AppInit() {
    std::cout << "SvrCubeWorld -- Initialize App" << std::endl;
    return true;
}