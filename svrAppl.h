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

class svrAppl {
    private:
        


    public:
        svrAppl();
        void createShader();
        int createWindow(unsigned int width, unsigned int height, char *name);
        int createWorld();
        void render();

        GLFWwindow* window = 0;
        Shader program;

        unsigned int vertexBuffer;
        unsigned int indexBuffer;
        unsigned int vertexArrayObject;

        VertexAttribs attribs;
        std::vector<TriangleIndex> indices;
        


};