



#include "svrAppl.h"

//OVR
#include "ovr/OVR_Math.h"
#include "ovr/VrApi_Helpers.h"

//STD
#include <iostream>
#include <vector>
#include <sys/resource.h>
//TAO
#include "test_i.h"
#include "tao/IORTable/IORTable.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"
#include "ServerWorker.h"


//TAO
int niterations = 10;
int nthreads = 1;



// GL Functions -- Resize Window Callback, ProcessInput
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(svrAppl appl);

// GL Window Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// TAO 
//----------------------------------------------
CORBA::ORB_var orb;
const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int main(int argc, char* argv[])
{
   
       orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&server_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Simple_Server_var server =
        Simple_Server::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      CORBA::Object_var table_object =
        orb->resolve_initial_references("IORTable");

      IORTable::Table_var table =
        IORTable::Table::_narrow (table_object.in ());
      if (CORBA::is_nil (table.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the IORTable.\n"),
                          1);
      table->bind ("Simple_Server", ior.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();
      Worker worker(orb.in());
      worker.activate(THR_NEW_LWP | THR_JOINABLE, nthreads);
     
    
    //Initialize the application
    svrAppl myAppl = svrAppl();
    
    myAppl.createWindow(SCR_WIDTH, SCR_HEIGHT, "CubeWorld");
    myAppl.createShader();
    myAppl.createWorld();
    myAppl.createCamera();
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(myAppl.window))
    {
        // input
        // -----
        processInput(myAppl);
        myAppl.updateView(server_impl.data[0], server_impl.data[1]);
        
        // Draw
        myAppl.render();

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
    
    const float cameraSpeed = 2.5f * appl.deltaTime; // adjust accordingly
    

    //TODO: THESE CONTROLS ARE BROKE
    /*
    if (glfwGetKey(appl.window, GLFW_KEY_W) == GLFW_PRESS) {
      appl.cameraPos += cameraSpeed * appl.cameraFront;
      //std::cout << "Forward" << std::endl;
    }
    if (glfwGetKey(appl.window, GLFW_KEY_S) == GLFW_PRESS) {
      appl.cameraPos -= cameraSpeed * appl.cameraFront;
    }
    if (glfwGetKey(appl.window, GLFW_KEY_A) == GLFW_PRESS)
       appl.cameraPos -= glm::normalize(glm::cross(appl.cameraFront, appl.cameraUp)) * cameraSpeed;
       
    if (glfwGetKey(appl.window, GLFW_KEY_D) == GLFW_PRESS)
        appl.cameraPos += glm::normalize(glm::cross(appl.cameraFront, appl.cameraUp)) * cameraSpeed;
    */

}
