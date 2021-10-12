//Needed for extensions
#define GL_GLEXT_PROTOTYPES

//GLFW
#include <GLFW/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ovr/OVR_Math.h"
#include "ovr/VrApi_Helpers.h"

//OVR Local
#include "svrCubeWorld.h"
#include <vector>

#include "shader_s.h"

//TODO: Can I get around the Jpeg and OpenCV?
//Convert to JPG
#include "extern/libjpeg-turbo/turbojpeg.h"
//Convert to h264
#include "extern/openh264/codec/api/svc/codec_api.h"
//Get information for h264
#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/mat.hpp>




// OVR VertexAttributes 
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




// CubeWorld Settings
// Set Number of Cubes and Variety of Rotations
// Highest I tried is 30k... Fails at 60k. Theoretical vertex max of Oculus 1,000,000 / 12 --> 83.333k
const int NUM_INSTANCES = 1500;
const int NUM_ROTATIONS = 16;

class svrAppl {
    private:
        


    public:
        svrAppl();
        void createShader();
        int createWindow(unsigned int width, unsigned int height, char *name);
        int createWorld();
        void createCamera();
        void createFramebuffer();
        void updateView(double xpos, double ypos);
        void render();
        void createImage();
        

        GLFWwindow* window = 0;
        unsigned int m_width;
        unsigned int m_height;
        Shader program;

        //Framebuffer
        Shader renderBufferShader;
        unsigned int quadVBO;
        unsigned int quadVAO;
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int rbo;
        int textureUnitIndex;

        unsigned char *pixels;
         long unsigned int jpegSize;
        unsigned char *header;
         long unsigned int headerSize;
        

        //World
        unsigned int vertexBuffer;
        unsigned int indexBuffer;
        unsigned int vertexArrayObject;

        VertexAttribs attribs;
        std::vector<TriangleIndex> indices;

        unsigned int InstanceTransformBuffer;
       unsigned int VertexTransformAttribute;

        // GLM MVP Matrices
        // -------------------
        glm::mat4 model = glm::mat4(1.0f);    
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        ovrVector3f CubePositions[NUM_INSTANCES];
        ovrVector3f Rotations[NUM_ROTATIONS];
        int CubeRotations[NUM_INSTANCES];

        float startTime;

        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;


        float deltaTime;
        float lastFrame;

        bool firstMouse;
        float yaw;
        float pitch;
        float lastX;
        float lastY;
        float fov;

        ISVCEncoder*  encoder_;
        SEncParamBase param;

       


};