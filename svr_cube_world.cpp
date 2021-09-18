#include <iostream>

#include <GLFW/glfw3.h>

// OVR
#include "Appl.h"


// Learn OpenGL
#include "shader_s.h"


using OVR::Vector4f;


int WIDTH = 800;
int HEIGHT = 600;

static const int NUM_INSTANCES = 1500;
static const int NUM_ROTATIONS = 16;

/*
The OVR Sample application doesn't actually use CLEAR_COLOR anywhere.
It gets declared and never used -- so it just must be some left over crud?
glClearColor gets called in a few places --
  -- Appl.cpp -> ovrAppl::AppEyeGLStateSetup
  -- SurfaceRenderApp.cpp --> ovrSurfaceRenderApp::AppEyeGLStateSetup

  It's unclear which one we're actually using, although adjusting the colors on them should make it clear. 
  TODO: Test which application glClearColor function that the OVRMSDK uses

*/
static const Vector4f CLEAR_COLOR(0.125f, 0.0f, 0.125f, 1.0f);



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

GLFWwindow* gl_create_window(int width, int height);




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

  GLFWwindow* window1 = gl_create_window(WIDTH, HEIGHT);
  //GLFWwindow* window2 = gl_create_window(WIDTH, HEIGHT);

  if (window1 == NULL) {      //} || window2 == NULL) {
    std::cout << "Failed to create one of the windows. Exiting." << std::endl;
    return -1;
  }

  //make window context
  glfwMakeContextCurrent(window1);
  //tell opengl what the viewport size is
  glViewport(0, 0, WIDTH, HEIGHT);
  //callback for resizing of frame
  glfwSetFramebufferSizeCallback(window1, framebuffer_size_callback);

  // render loop
  while (!glfwWindowShouldClose(window1))
    {
      // input
      processInput(window1);

      // rendering commands here
      //....
      // at the start of the frame, we want to clear the screen
      glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, CLEAR_COLOR.w);
      glClear(GL_COLOR_BUFFER_BIT);

      // check and call events and swap buffers
      // we have TWO BUFFERS -- front is displayed, back is being rendered
      // then we swap to prevent flickering
      glfwSwapBuffers(window1);
      glfwPollEvents();
    }

  glfwTerminate();
  
  return 0;
}

// Utility function to help us create multiple windows and check for any errors
GLFWwindow* gl_create_window(int width, int height) {
  GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);

  if (window == NULL)
    {
      std::cout << "Error creating window" << std::endl;
      glfwTerminate();
      return NULL;
    }
    return window;

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

