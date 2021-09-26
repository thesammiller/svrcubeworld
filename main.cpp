#define GL_GLEXT_PROTOTYPES

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "ovr/OVR_Bridge.h"
#include "ovr/OVR_Math.h"
#include "ovr/VrApi_Helpers.h"

#include "gl_helper.h"

#include "shader_s.h"

#include <iostream>
#include <vector>
#include <sys/resource.h>


const int NUM_INSTANCES = 1500;
const int NUM_ROTATIONS = 16;




#define GL(func) func;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct VertexAttribs {
    std::vector<OVR::Vector3f> position;
    std::vector<OVR::Vector3f> normal;
    std::vector<OVR::Vector3f> tangent;
    std::vector<OVR::Vector3f> binormal;
    std::vector<OVR::Vector4f> color;
    std::vector<OVR::Vector2f> uv0;
    std::vector<OVR::Vector2f> uv1;
    std::vector<OVR::Vector4i> jointIndices;
    std::vector<OVR::Vector4f> jointWeights;
};

typedef uint16_t TriangleIndex;

template <typename _attrib_type_>
void PackVertexAttribute(
    std::vector<uint8_t>& packed,
    const std::vector<_attrib_type_>& attrib,
    const int glLocation,
    const int glType,
    const int glComponents) {
    if (attrib.size() > 0) {
        const size_t offset = packed.size();
        const size_t size = attrib.size() * sizeof(attrib[0]);

        packed.resize(offset + size);
        memcpy(&packed[offset], attrib.data(), size);

        glEnableVertexAttribArray(glLocation);
        glVertexAttribPointer(
            glLocation, glComponents, glType, false, sizeof(attrib[0]), (void*)(offset));
    } else {
        glDisableVertexAttribArray(glLocation);
    }
}


enum VertexAttributeLocation {
    VERTEX_ATTRIBUTE_LOCATION_POSITION = 0,
    VERTEX_ATTRIBUTE_LOCATION_COLOR = 1,
    VERTEX_ATTRIBUTE_LOCATION_ROTATION = 2
};


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CubeWorld", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader program("shaders/vertexShader.vs", "shaders/fragmentShader.fs");


    int VERTICES_PER_OBJECT = 8; //8
    VertexAttribs attribs;
    attribs.position.resize(VERTICES_PER_OBJECT);
    attribs.color.resize(VERTICES_PER_OBJECT);
    for (int i = 0; i < VERTICES_PER_OBJECT; i++) {
        attribs.position[i] = cubeVertices.positions[i];
        attribs.color[i] = cubeVertices.colors[i];
    }

    std::vector<TriangleIndex> indices;
    int NUMBER_OF_INDICES = 36; //36
    indices.resize(NUMBER_OF_INDICES);
    for (int i = 0; i < NUMBER_OF_INDICES; i++) {
        indices[i] = cubeIndices[i];
    }

    //GlGeometry Create Function
    int vertexCount = attribs.position.size();
    int indexCount = indices.size();

    std::vector<OVR::Vector3f> position;
    std::vector<OVR::Vector3f> normal;
    std::vector<OVR::Vector3f> tangent;
    std::vector<OVR::Vector3f> binormal;

    unsigned int vertexBuffer; //VBO
    unsigned int indexBuffer, vertexArrayObject;

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glBindAttribLocation(program.ID, VERTEX_ATTRIBUTE_LOCATION_COLOR, "VertexColor");    
    
    std::vector<uint8_t> packed;
    PackVertexAttribute(
        packed, attribs.position, VERTEX_ATTRIBUTE_LOCATION_POSITION, GL_FLOAT, 3);

    PackVertexAttribute(packed, attribs.color, VERTEX_ATTRIBUTE_LOCATION_COLOR, GL_FLOAT, 4);


    glBufferData(GL_ARRAY_BUFFER, packed.size() * sizeof(packed[0]), packed.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(indices[0]),
        indices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);

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
    

    unsigned int InstanceTransformBuffer;
    unsigned int VertexTransformAttribute;
    VertexTransformAttribute = glGetAttribLocation(program.ID, "VertexTransform");

    float startTime = glfwGetTime();
   
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        program.use();

        float time = glfwGetTime();


        // OVR CubeWorld Game Logic Updates -- update instance transforms
        // ---------------------------------
        
        glBindVertexArray(vertexArrayObject);

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
        unsigned int viewLoc = glGetUniformLocation(program.ID,"view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(program.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // GL DRAW CALL
        // ------------------
        GL(glBindVertexArray(vertexArrayObject));
        GL(glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL, NUM_INSTANCES));

        GL(glBindVertexArray(0));
        GL(glUseProgram(0));


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    /*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    */

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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

