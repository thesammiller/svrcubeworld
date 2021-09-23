#define STB_IMAGE_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include <cstdio>
#include "shader_s.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 projection;"

  "void main()\n"
  "{ "
  //remember we read the multiplication from right to left
  "gl_Position = projection * view * model * vec4(aPos, 1.0);"
  "ourColor = aColor;"
  "TexCoord = aTexCoord;"
  "}\n";

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec3 ourColor;"
  "in vec2 TexCoord;"
  "uniform float mixColor;\n"
  "uniform sampler2D texture1;"
  "uniform sampler2D texture2;"
  "void main()\n"
  "{\n"
    "FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(2 * TexCoord.x, 2 *  TexCoord.y)), mixColor);"
  "}";


int main(void)
{
  glEnable(GL_DEPTH_TEST);
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

  //---------------------------------------------

  unsigned int texture1, texture2;
  stbi_set_flip_vertically_on_load(true);  


  // texture 1
  glGenTextures(1, &texture1); 
  glBindTexture(GL_TEXTURE_2D, texture1); 
  // TEXTURE wrap
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //texture filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //load image
  int width, height, nrChannels;
  unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  // create texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);

  // texture 2
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

    // TEXTURE wrap
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // filter
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char *data2 = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
  if (data2)
  {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
      glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data2);

  glUseProgram(shaderProgram); //activate the shader before using uniforms

  glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0); // set it manually
  glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);


  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

  
  // Model Matrix
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  // View Matrix
  glm::mat4 view = glm::mat4(1.0f);
  // note that we're translating the scene in the reverse direction
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  // PROJECTION MATRIX
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);




  // render loop
  while (!glfwWindowShouldClose(window))
    {
      // input
      processInput(window);
      
      int modelValue = glGetUniformLocation(shaderProgram, "model");
      int viewValue = glGetUniformLocation(shaderProgram, "view");
      int projectionValue = glGetUniformLocation(shaderProgram, "projection");

      // GLM -----------------------------------------------------------
      glm::mat4 trans = glm::mat4(1.0f);
      
      trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
      trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

      //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
      //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

      //MODEL ROTATES OVER TIME
      //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));



      // rendering commands here

      // at the start of the frame, we want to clear the screen
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      //bind textures to texture units
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);

      //render contianer
       glUseProgram(shaderProgram); //activate the shader before using uniforms
      float timeValue = glfwGetTime();
      float mixValue = (sin(timeValue) / 2.0f) + 0.5f;
      
      
      glUniformMatrix4fv(modelValue, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(viewValue, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projectionValue, 1, GL_FALSE, glm::value_ptr(projection));
      

      glEnable(GL_DEPTH_TEST);



      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      // For indexed cube... 
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


      //Second Triangle -- Exercise
      //trans = glm::mat4(1.0f);
      
      //trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
      //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

      //trans = glm::scale(trans, glm::vec3(sin( (float) glfwGetTime()), sin( (float) glfwGetTime() ), 0.5));
      //glUniformMatrix4fv(transformValue, 1, GL_FALSE, glm::value_ptr(trans));
      //      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


      
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
