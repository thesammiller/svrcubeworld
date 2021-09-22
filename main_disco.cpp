#define STB_IMAGE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <cstdio>
#include "shader_s.h"
#include "stb_image.h"

// Vertices for a Rectangle
// Normalized Z to 0 to discard depth
// 0,0 as center of screen
float vertices[] = {
   1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //top right
   1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //bottom right
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //bottom left
  -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //top left
};

unsigned int indices[] = { // note we start from index 0 
  0, 1, 3, 
  1, 2, 3
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "layout (location = 1) in vec3 aColor;"
  "layout (location = 2) in vec2 aTexCoord;"
  "out vec3 ourColor;"
  "out vec2 TexCoord;"
  "void main()\n"
  "{ "
  "gl_Position = vec4(aPos, 1.0);"
  "ourColor = aColor;"
  "TexCoord = aTexCoord;"
  "}\n";

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;"
  "in vec2 TexCoord;"
  "uniform sampler2D ourTexture;"
  "void main()\n"
  "{\n"
  "FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);\n"
  "}";


int main(void)
{
  //initialize the library
  glfwInit();
  //window hints for version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  //for mac -->
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

  if (window == NULL)
    {
      printf("Error creting window");
      glfwTerminate();
      return -1;
    }

  //make window context
  glfwMakeContextCurrent(window);
  //tell opengl what the viewport size is
  glViewport(0, 0, 800, 600);
  //callback for resizing of frame
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



  // Store Vertex Shader
  unsigned int vertexShader;
  // create shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // Load the source code for the shader
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  //compile shader
  glCompileShader(vertexShader);

  //will now want to check for success on the shader compilation
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Error with the shader");
    printf("%s", infoLog);
  }

  // FRAGMENT SHADER
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Check for compile errors.
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    printf("Fragment shader not successful");
  }

  // create a program object
  // to link the output of vector to the input of fragment shader
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    printf("Error with the program linker.");
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // unique IDs
   unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  // TEXTURE LOGIC
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char *data = stbi_load("container.jpg", &width, &height,
  &nrChannels, 0);

  unsigned int texture;
  glGenTextures(1, &texture); 

  glBindTexture(GL_TEXTURE_2D, texture);  
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);



  // render loop
  while (!glfwWindowShouldClose(window))
    {
      // input
      processInput(window);
      

      // rendering commands here

      // at the start of the frame, we want to clear the screen
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);



      // Now we can use the shader program
      glUseProgram(shaderProgram);
      

      // VERTEX ARRAY OBJECT -- bound and will store all the stuff
      // opengl requires a VAO
      glBindTexture(GL_TEXTURE_2D, texture);
      glBindVertexArray(VAO);
      //glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      
      // check and call events and swap buffers
      // we have TWO BUFFERS -- front is displayed, back is being rendered
      // then we swap to prevent flickering
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}


void processInput (GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
