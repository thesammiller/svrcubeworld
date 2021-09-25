#define STB_IMAGE_IMPLEMENTATION


#include "OVR_Math.h"
#include "VrApi_Helpers.h"

#include <GLFW/glfw3.h>
#include "stb_image.h"




#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

#include <iostream>
#include <vector>
#include <sys/resource.h>


using OVR::Vector3f;
using OVR::Vector4f;

const int NUM_INSTANCES = 150;
const int NUM_ROTATIONS = 16;

ovrVector3f* Rotations = (ovrVector3f*) malloc(sizeof(Vector3f) * NUM_ROTATIONS);
ovrVector3f* CubePositions = (ovrVector3f*) malloc (sizeof(Vector3f) * NUM_INSTANCES);
int CubeRotations[NUM_INSTANCES];
    

#define GL(func) func;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 10.3 Walk Around
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

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

unsigned int Random;    
float RandomFloat() {
    Random = 1664525L * Random + 1013904223L;
    unsigned int rf = 0x3F800000 | (Random & 0x007FFFFF);
    return (*(float*)&rf) - 1.0f;
}
/*
================================================================================

ovrGeometry

================================================================================
*/
typedef struct {
    GLint Index;
    GLint Size;
    GLenum Type;
    GLboolean Normalized;
    GLsizei Stride;
    const GLvoid* Pointer;
} ovrVertexAttribPointer;


#define MAX_VERTEX_ATTRIB_POINTERS 3
typedef struct {
    GLuint VertexBuffer;
    GLuint IndexBuffer;
    GLuint VertexArrayObject;
    int VertexCount;
    int IndexCount;
    ovrVertexAttribPointer VertexAttribs[MAX_VERTEX_ATTRIB_POINTERS];
} ovrGeometry;

/*
enum VertexAttributeLocation {
    VERTEX_ATTRIBUTE_LOCATION_POSITION = 0,
    VERTEX_ATTRIBUTE_LOCATION_NORMAL = 1,
    VERTEX_ATTRIBUTE_LOCATION_TANGENT = 2,
    VERTEX_ATTRIBUTE_LOCATION_BINORMAL = 3,
    VERTEX_ATTRIBUTE_LOCATION_COLOR = 4,
    VERTEX_ATTRIBUTE_LOCATION_UV0 = 5,
    VERTEX_ATTRIBUTE_LOCATION_UV1 = 6,
    VERTEX_ATTRIBUTE_LOCATION_JOINT_INDICES = 7,
    VERTEX_ATTRIBUTE_LOCATION_JOINT_WEIGHTS = 8,
    VERTEX_ATTRIBUTE_LOCATION_FONT_PARMS = 9
};
*/


enum VertexAttributeLocation {
    VERTEX_ATTRIBUTE_LOCATION_POSITION = 0,
    VERTEX_ATTRIBUTE_LOCATION_COLOR = 1,
    VERTEX_ATTRIBUTE_LOCATION_ROTATION = 2
};




// Returns a random float in the range [0, 1].
static float ovrScene_RandomFloat(int) {
    return (float) ( rand() ) / RAND_MAX;
}



// setup Cube
struct ovrCubeVertices {
    Vector3f positions[8];
    Vector4f colors[8];
};

static ovrCubeVertices cubeVertices = {
    // positions
    {
        Vector3f(-1.0f, +1.0f, -1.0f),
        Vector3f(+1.0f, +1.0f, -1.0f),
        Vector3f(+1.0f, +1.0f, +1.0f),
        Vector3f(-1.0f, +1.0f, +1.0f), // top
        Vector3f(-1.0f, -1.0f, -1.0f),
        Vector3f(-1.0f, -1.0f, +1.0f),
        Vector3f(+1.0f, -1.0f, +1.0f),
        Vector3f(+1.0f, -1.0f, -1.0f) // bottom
    },
    // colors
    {Vector4f(1.0f, 0.0f, 1.0f, 1.0f),
     Vector4f(0.0f, 1.0f, 0.0f, 1.0f),
     Vector4f(0.0f, 0.0f, 1.0f, 1.0f),
     Vector4f(1.0f, 0.0f, 0.0f, 1.0f),
     Vector4f(0.0f, 0.0f, 1.0f, 1.0f),
     Vector4f(0.0f, 1.0f, 0.0f, 1.0f),
     Vector4f(1.0f, 0.0f, 1.0f, 1.0f),
     Vector4f(1.0f, 0.0f, 0.0f, 1.0f)},
};

static const unsigned short cubeIndices[36] = {
    0, 2, 1, 2, 0, 3, // top
    4, 6, 5, 6, 4, 7, // bottom
    2, 6, 7, 7, 1, 2, // right
    0, 4, 5, 5, 3, 0, // left
    3, 5, 6, 6, 2, 3, // front
    0, 1, 7, 7, 4, 0 // back
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    Shader program("vertexShader.vs", "fragmentShader.fs");


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

    glm::mat4 model = glm::mat4(1.0f);
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(90.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 10000.0f);

    ovrVector3f CubePositions[NUM_INSTANCES];
    ovrVector3f Rotations[NUM_ROTATIONS];

    for (int i = 0; i < NUM_ROTATIONS; i++) {
        Rotations[i].x = RandomFloat();
        Rotations[i].y = RandomFloat();
        Rotations[i].z = RandomFloat();
    }

    for (int i = 0; i < NUM_INSTANCES; i++) {
        volatile float rx, ry, rz;
        for (;;) {
            rx = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));
            ry = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));
            rz = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));

            
            // If too close to 0,0,0
            if (fabsf(rx) < 4.0f && fabsf(ry) < 4.0f && fabsf(rz) < 4.0f) {
                continue;
            }

            // Test for overlap with any of the existing cubes.
            bool overlap = false;
            for (int j = 0; j < i; j++) {
                if (fabsf(rx - CubePositions[j].x) < 4.0f && //4
                    fabsf(ry - CubePositions[j].y) < 4.0f &&
                    fabsf(rz - CubePositions[j].z) < 4.0f) {
                    overlap = true;
                    break;
                }
            }

            if (!overlap) {
                break;
            }
        }

        //rx *= 0.1f;
        //ry *= 0.1f;
        //rz *= 0.1f;

        // Insert into list sorted based on distance.
        int insert = 0;
        const float distSqr = rx * rx + ry * ry + rz * rz;
        for (int j = i; j > 0; j--) {
            ovrVector3f* otherPos = &CubePositions[j - 1];
            const float otherDistSqr =
                otherPos->x * otherPos->x + otherPos->y * otherPos->y + otherPos->z * otherPos->z;
            if (distSqr > otherDistSqr) {
                insert = j;
                break;
            }
            CubePositions[j] = CubePositions[j - 1];
            CubeRotations[j] = CubeRotations[j - 1];
        }

        CubePositions[insert].x = rx;
        CubePositions[insert].y = ry;
        CubePositions[insert].z = rz;

        CubeRotations[insert] = (int)(RandomFloat() * (NUM_ROTATIONS - 0.1f));
    }
    unsigned int InstanceTransformBuffer;
    unsigned int VertexTransformAttribute;
    VertexTransformAttribute = glGetAttribLocation(program.ID, "VertexTransform");

    float startTime = glfwGetTime();

    // Update the instance transform attributes.
   

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


        Vector3f currentRotation;
        currentRotation.x = (float)(time - startTime);
        currentRotation.y = (float)(time - startTime);
        currentRotation.z = (float)(time - startTime);
    

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
        //GL(glBindVertexArray(0));
        
         ovrMatrix4f rotationMatrices[NUM_ROTATIONS];
        for (int i = 0; i < NUM_ROTATIONS; i++) {
            rotationMatrices[i] = ovrMatrix4f_CreateRotation(
                Rotations[i].x * currentRotation.x,
                Rotations[i].y * currentRotation.y,
                Rotations[i].z * currentRotation.z);
        }

        GL(glBindBuffer(GL_ARRAY_BUFFER, InstanceTransformBuffer));

        GL(ovrMatrix4f* cubeTransforms = (ovrMatrix4f*)glMapBufferRange(
            GL_ARRAY_BUFFER,
            0,
            NUM_INSTANCES * sizeof(ovrMatrix4f),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
        for (int i = 0; i < NUM_INSTANCES; i++) {
            const int index = CubeRotations[i];

            // Write in order in case the mapped buffer lives on write-combined memory.
            cubeTransforms[i].M[0][0] = rotationMatrices[index].M[0][0];
            cubeTransforms[i].M[0][1] = rotationMatrices[index].M[0][1];
            cubeTransforms[i].M[0][2] = rotationMatrices[index].M[0][2];
            cubeTransforms[i].M[0][3] = rotationMatrices[index].M[0][3];

            cubeTransforms[i].M[1][0] = rotationMatrices[index].M[1][0];
            cubeTransforms[i].M[1][1] = rotationMatrices[index].M[1][1];
            cubeTransforms[i].M[1][2] = rotationMatrices[index].M[1][2];
            cubeTransforms[i].M[1][3] = rotationMatrices[index].M[1][3];

            cubeTransforms[i].M[2][0] = rotationMatrices[index].M[2][0];
            cubeTransforms[i].M[2][1] = rotationMatrices[index].M[2][1];
            cubeTransforms[i].M[2][2] = rotationMatrices[index].M[2][2];
            cubeTransforms[i].M[2][3] = rotationMatrices[index].M[2][3];

            cubeTransforms[i].M[3][0] = CubePositions[i].x;
            cubeTransforms[i].M[3][1] = CubePositions[i].y;
            cubeTransforms[i].M[3][2] = CubePositions[i].z;
            cubeTransforms[i].M[3][3] = 1.0f;
        }
        GL(glUnmapBuffer(GL_ARRAY_BUFFER));

        GL(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
        GL(glScissor(0, 0, SCR_WIDTH, SCR_HEIGHT));
        GL(glClearColor(0.016f, 0.0f, 0.016f, 1.0f));
        GL(glEnable(GL_FRAMEBUFFER_SRGB_EXT));
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL(glDisable(GL_FRAMEBUFFER_SRGB_EXT));

        unsigned int viewLoc = glGetUniformLocation(program.ID,"view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(program.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //std::cout << "Moving mouse" << std::endl;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
        
    float xoffset = xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if(pitch > 89.0f)
    pitch = 89.0f;
    if(pitch < -89.0f)
    pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}
