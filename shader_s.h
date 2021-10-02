#pragma once

//Needed for extensions
#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GL(func) func

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
 public:
  Shader();
  //program ID
  unsigned int ID;

  //Constructor reads and builds the shader
  Shader(const char* vertexPath, const char* fragmentPath);
  //use activet the shder
  void use();
  //utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, glm::mat4 mat);
};
