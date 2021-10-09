#include "gtest/gtest.h"

//TODO: Rewrite this test to work with the new class system.
/*

//height
//width

struct GLsetup {
  GLFWwindow* window;
};

struct GLWindowTest : testing::Test 
{
  GLsetup* setup;

  GLWindowTest() 
  {
    setup = new GLsetup;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    setup->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLWindowTest", NULL, NULL);
  }
  ~GLWindowTest()
  {
    delete setup;
  }

};

// Test that mouse callback repositions the Camera Front
TEST_F(GLWindowTest, GLMouseLookAtTest) {
  cameraFront = glm::vec3(1.0f);
  glm::vec3 oldCameraFront = glm::vec3(1.0f);
  mouse_callback(setup->window, 1.0, 2.0);
  ASSERT_NE(cameraFront, oldCameraFront);
  
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/