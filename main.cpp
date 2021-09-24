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
using OVR::Matrix4f;

const int NUM_INSTANCES = 150;
const int NUM_ROTATIONS = 16;

Vector3f* Rotations = (Vector3f*) malloc(sizeof(Vector3f) * NUM_ROTATIONS);
Vector3f* CubePositions = (Vector3f*) malloc (sizeof(Vector3f) * NUM_INSTANCES);
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

void print_matrix(ovrMatrix4f *mat) {
    std::cout << "----------" << std::endl;
    for (int x=0; x < 4; x++) {
            for (int y=0; y < 4; y++) {
                std::cout << (*mat).M[x][y] << "\t";
            }
            std::cout << std::endl;
        }
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
    VERTEX_ATTRIBUTE_LOCATION_COLOR = 1
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


    glBufferData(GL_ARRAY_BUFFER, packed.size() * sizeof(packed[0]), packed.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(indices[0]),
        indices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);

    ovrMatrix4f model = ovrMatrix4f_CreateIdentity();
    
    ovrMatrix4f view = ovrMatrix4f_CreateIdentity();
    //glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    /*
    ovrMatrix4f_Multiply(view, poseM) --> poseM is the return from GetTransformFroMPose
    static inline ovrMatrix4f vrapi_GetTransformFromPose(const ovrPosef* pose) {
    const ovrMatrix4f rotation = ovrMatrix4f_CreateFromQuaternion(&pose->Orientation);
    const ovrMatrix4f translation =
        ovrMatrix4f_CreateTranslation(pose->Position.x, pose->Position.y, pose->Position.z);
    return ovrMatrix4f_Multiply(&translation, &rotation);
    }
    */
    


    ovrMatrix4f projection;
    //glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
    projection = ovrMatrix4f_CreateProjectionFov(90.0f, 90.0f, 0.0f, 0.0f, 0.1f, 100.0f);
    
    

    

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
        ovrMatrix4f rotationMatrix = ovrMatrix4f_CreateRotation(0.0f, 0.0f, 0.0f);
        model = ovrMatrix4f_Multiply(&model, &rotationMatrix);

        //Using sample data from the head pose... 
        ovrQuatf quat; 
        quat.x = 0.0f; //0.131918f;
        quat.y = 0.0f; //-0.019500f;
        quat.z = 0.0f; //0.001918f;
        quat.w = 1.0f; //0.991067f;
        //const ovrMatrix4f rotation = ovrMatrix4f_CreateFromQuaternion(&quat);
        //const ovrMatrix4f translation = ovrMatrix4f_CreateTranslation(0.0f, 0.0f, 0.05f);
        //ovrMatrix4f_CreateTranslation(0.005f, 1.0f, 0.05f);


        ovrPosef pose = {};
        pose.Position.x = 0.0f;
        pose.Position.y = 0.0f;
        pose.Position.z = -3.0f;
        pose.Orientation.x = 0.0f;
        pose.Orientation.y = 0.0f;
        pose.Orientation.z = 0.0f;
        pose.Orientation.w = 1.0f;
        ovrMatrix4f poseM = vrapi_GetTransformFromPose(&pose);
        
        print_matrix(&poseM);
        
        ovrMatrix4f centerEyeViewMatrix = vrapi_GetViewMatrixFromPose(&pose);
        print_matrix(&centerEyeViewMatrix);
        view =  ovrMatrix4f_Multiply(&centerEyeViewMatrix, &poseM); //ovrMatrix4f_CreateTranslation(0.0f, 0.0f, 0.0f );
        
        //view =  ovrMatrix4f_Multiply(&view, &translationMatrix);
    

        unsigned int modelLoc = glGetUniformLocation(program.ID,"model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model.M[0][0]);
        
  
        unsigned int viewLoc = glGetUniformLocation(program.ID,"view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view.M[0][0]);

        unsigned int projectionLoc = glGetUniformLocation(program.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection.M[0][0]);
  

        GL(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
        GL(glScissor(0, 0, SCR_WIDTH, SCR_HEIGHT));
        GL(glClearColor(0.016f, 0.0f, 0.016f, 1.0f));
        GL(glEnable(GL_FRAMEBUFFER_SRGB_EXT));
        GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL(glDisable(GL_FRAMEBUFFER_SRGB_EXT));
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
