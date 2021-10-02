

#include "svrAppl.h"

//OVR
#include "ovr/OVR_Math.h"
#include "ovr/VrApi_Helpers.h"

//STD
#include <iostream>
#include <vector>
#include <sys/resource.h>

#include "svrServer.h"



//TAO
int niterations = 10;
int nthreads = 1;

// GL Functions -- Resize Window Callback, ProcessInput
//TODO I think this first one can be removed
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(svrAppl appl);

// GL Window Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// TAO 
//----------------------------------------------



int main(int argc, char* argv[])
{
      //Server myServer;
      //Initialize orb object
      //myServer.createOrb();
      //Spin off a worker
      //myServer.configServer();
      //myServer.createWorker()
   
    svrServer myServer = svrServer();
    myServer.createOrb(argc, argv);
    myServer.createServer();
     
    
    //Initialize the application
    svrAppl myAppl = svrAppl();
    // Create main window
    myAppl.createWindow(SCR_WIDTH, SCR_HEIGHT, "CubeWorld");
    //Create shader program for GPU
    //TODO: Pass in shader file parameters
    myAppl.createShader();
    // Create the CubeWorld vertex data
    myAppl.createWorld();
    // Create the GLM Matrices for Cameras, set camera position
    myAppl.createCamera();

    Worker worker(myServer.orb.in());
    worker.activate(THR_NEW_LWP | THR_JOINABLE, nthreads);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(myAppl.window))
    {
        // input
        // -----
        //This just checks for the escape key (which should trigger window should close)
        //TODO: This just kills the world... the hanging thread below needs to be fixed.
        processInput(myAppl);

        //myServer.getData();

        // Take the server data and update the view based on controller input
        myAppl.updateView(myServer.server_impl.data[0], myServer.server_impl.data[1]);
        
        // Draw
        myAppl.render();

        //TODO: Send drawing to client
        //myServer.sendDrawing();

    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    /*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    */

    //TODO: This causes a hang, need to fix.
    worker.thr_mgr() -> wait();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(svrAppl appl)
{
    //Escape key works... 
    if (glfwGetKey(appl.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {    
        glfwSetWindowShouldClose(appl.window, true);
    }

}
