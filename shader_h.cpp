
#include "shader_s.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
  // 1. retrive the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  //ensure ifstream objects can throw exceptions
  vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try
    {
      // open files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      //read file's buffer content into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      //close file handlers
      vShaderFile.close();
      fShaderFile.close();
      //convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
  catch (std::ifstream::failure e)
    {
      std::cout << "ERROR:SHADER::FILE_NOT_SUCCESSFULLY_READ)" << std::endl;
    }

  vShaderCode = vertexCode.c_str();
  fShaderCode = fragmentCode.c_str();

};
