
#include <GLFW/glfw3.h>

#include "OVR_Math.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using OVR::Matrix4f;

class Shader
{
 public:
  Shader(const char* vertexPath, const char* fragmentPath);

  //program ID
  unsigned int ID;
  const char* vShaderCode;
  const char* fShaderCode;

  
  
};
