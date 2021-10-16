

#include "svr/svrAppl.h"

//OVR
#include "ovr/OVR_Math.h"
#include "ovr/VrApi_Helpers.h"

//STD
#include <iostream>
#include <vector>
#include <sys/resource.h>

#include "svr/svrServer.h"




//TAO
int niterations = 10;
int nthreads = 1;

//Function to catch escape key
void processInput(svrAppl appl);

// GL Window Settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;



int main(int argc, char* argv[])
{

    
   
    svrServer myServer = svrServer();
    //Create orb
    myServer.createOrb(argc, argv);
    //Create server implementation
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
    myAppl.createFramebuffer();

    //Create a worker thread to run asynchronously
    //Worker worker(myServer.orb.in());
    //Activate the worker thread
    //worker.activate(THR_NEW_LWP | THR_JOINABLE, nthreads);

    float startTime = glfwGetTime();

    Worker worker(myServer.orb.in());

     if (worker.activate (THR_NEW_LWP | THR_JOINABLE, nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Cannot activate worker threads\n"),
                          1);


    int frame = 0;
    float old_time = 0;
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(myAppl.window))
    {

        // input
        // -----
        //This just checks for the escape key (which should trigger window should close)
        //TODO: This just kills the world... the hanging thread below needs to be fixed.
        processInput(myAppl);

        //Controller Input
        // -----------------
        // Take the server data and update the view based on controller input
        
        myServer.server_impl.get_data(myAppl.headpose_data);
        
        // OpenGL Draw & Create Image
        // --------------
        myAppl.render();

        // TAO Server Set Data
        // --------------------
        //Take the data from OpenGL and set Server Data
        //TODO: Are the classes in an appropriate relationship?
        myServer.setJpegSize(myAppl.jpegSize);
        myServer.setImage(myAppl.pixels);
        
        myServer.setHeaderSize(myAppl.headerSize);
        myServer.setHeader(myAppl.header);
        
        // Calculate Frame Rate
        // -------------------
        if ( (old_time + 1) < glfwGetTime() ) {
            std::cout << "FPS " << frame << std::endl;
            frame = 0;
            old_time = glfwGetTime();
        }
        ++frame;

        

    }

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
