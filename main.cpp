

//GL Local
#include "gl_helper.h"

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

// CubeWorld Settings
// Set Number of Cubes and Variety of Rotations
// Highest I tried is 30k... Fails at 60k. Theoretical vertex max of Oculus 1,000,000 / 12 --> 83.333k
const int NUM_INSTANCES = 1500;
const int NUM_ROTATIONS = 16;

//TAO
int niterations = 10;
int nthreads = 1;


// GL Debugging Wrapper -- no-op
#define GL(func) func;

// GL Functions -- Resize Window Callback, ProcessInput
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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


    // GLM MVP Matrices
    // -------------------
    glm::mat4 model = glm::mat4(1.0f);    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection;

    projection = glm::perspective(glm::radians(90.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 10000.0f);


    // OVR Game Logic Setup
    // ------------------------
    ovrVector3f CubePositions[NUM_INSTANCES];
    ovrVector3f Rotations[NUM_ROTATIONS];
    int CubeRotations[NUM_INSTANCES];

    generate_random_rotations(Rotations, NUM_ROTATIONS);
    generate_random_locations(CubePositions, CubeRotations, NUM_INSTANCES, Rotations, NUM_ROTATIONS);
    
    // Get the attribute for the instance transforms
    unsigned int InstanceTransformBuffer;
    unsigned int VertexTransformAttribute;
    VertexTransformAttribute = glGetAttribLocation(myAppl.program.ID, "VertexTransform");

    float startTime = glfwGetTime();

    
   
    // render loop
    // -----------
    while (!glfwWindowShouldClose(myAppl.window))
    {
        // input
        // -----
        processInput(myAppl.window);
        mouse_callback(myAppl.window, server_impl.data[0], server_impl.data[1]);

        // render
        // ------
        myAppl.program.use();

        float time = glfwGetTime();


        // OVR CubeWorld Game Logic Updates -- update instance transforms
        // ---------------------------------
        
        glBindVertexArray(myAppl.vertexArrayObject);

        GL(glGenBuffers(1, &InstanceTransformBuffer));
        GL(glBindBuffer(GL_ARRAY_BUFFER, InstanceTransformBuffer));
        GL(glBufferData(
            GL_ARRAY_BUFFER, NUM_INSTANCES * 4 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW));
        
        for (int i = 0; i < 4; i++) {
            GL(glEnableVertexAttribArray(VertexTransformAttribute + i));
            GL(glVertexAttribPointer(
                VertexTransformAttribute + i,
                4,
                GL_FLOAT,
                false,
                4 * 4 * sizeof(float),
                (void*)(i * 4 * sizeof(float))));
            GL(glVertexAttribDivisor(VertexTransformAttribute + i, 1));
        }
            
        GL(glBindBuffer(GL_ARRAY_BUFFER, InstanceTransformBuffer));

        GL(ovrMatrix4f* cubeTransforms = (ovrMatrix4f*)glMapBufferRange(
            GL_ARRAY_BUFFER,
            0,
            NUM_INSTANCES * sizeof(ovrMatrix4f),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

        update_cube_rotations(startTime, time, Rotations, NUM_ROTATIONS, cubeTransforms, NUM_INSTANCES, CubeRotations, CubePositions); 
        
        GL(glUnmapBuffer(GL_ARRAY_BUFFER));

        // GL Clear Screen and Prepare Draw
        // --------------------------------
        GL(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
        GL(glScissor(0, 0, SCR_WIDTH, SCR_HEIGHT));
        GL(glClearColor(0.016f, 0.0f, 0.016f, 1.0f));
        GL(glEnable(GL_FRAMEBUFFER_SRGB_EXT));
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL(glDisable(GL_FRAMEBUFFER_SRGB_EXT));

        // GLM Update Camera Postion
        // -------------------------
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        unsigned int viewLoc = glGetUniformLocation(myAppl.program.ID,"view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(myAppl.program.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // GL DRAW CALL
        // ------------------
        GL(glBindVertexArray(myAppl.vertexArrayObject));
        GL(glDrawElementsInstanced(GL_TRIANGLES, myAppl.indices.size(), GL_UNSIGNED_SHORT, NULL, NUM_INSTANCES));

        GL(glBindVertexArray(0));
        GL(glUseProgram(0));


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(myAppl.window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    /*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    */
    worker.thr_mgr() -> wait();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    //TODO: Process input from TAO
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {    
        glfwSetWindowShouldClose(window, true);
    }
    
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      cameraPos += cameraSpeed * cameraFront;
      //std::cout << "Forward" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
       cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
       
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}
