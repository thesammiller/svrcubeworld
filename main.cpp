

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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


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
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(myAppl.window))
    {
        if (myServer.orb->work_pending()) {
            myServer.orb->perform_work();

        }

        
        
        // input
        // -----
        //This just checks for the escape key (which should trigger window should close)
        //TODO: This just kills the world... the hanging thread below needs to be fixed.
        processInput(myAppl);

        //myServer.getData();

        // Take the server data and update the view based on controller input
        float in_data[7];
        myServer.server_impl.get_data(in_data);
        myAppl.updateView(in_data[0], in_data[1]);
        
        // Draw
        myAppl.render();
        myAppl.createImage();
        myServer.server_impl.setJpegSize(myAppl.jpegSize);
        //TODO: SAVE RENDER TO PIXELS ON THE SERVER
        // SO THAT WHEN CLIENT REQUESTS DATA, IT'S THERE
        unsigned char* m_pixels = (unsigned char*) malloc (myAppl.jpegSize);
        
        memcpy(m_pixels, myAppl.pixels, sizeof(unsigned char) * myAppl.jpegSize);
        myServer.setImage(m_pixels, myAppl.jpegSize);

        delete(m_pixels);

        


    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    /*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    */

    //TODO: This causes a hang, need to fix.
    //worker.thr_mgr() -> wait();

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
