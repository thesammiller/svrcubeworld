#include "svrAppl.h"
#include <iostream>



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



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


svrAppl::svrAppl() {

}



int svrAppl::createWindow(unsigned int width, unsigned int height, char *name) {
// glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return 1;

}

void svrAppl::init() {
     glEnable(GL_DEPTH_TEST);

    //build and compile shader
    program = Shader("shaders/vertexShader.vs", "shaders/fragmentShader.fs");


}

int svrAppl::createWorld() {

    //CubeWorld Setup
    //Convert Vertex Struct to Vector of Vertexes
    int VERTICES_PER_OBJECT = 8; //8
    
    attribs.position.resize(VERTICES_PER_OBJECT);
    attribs.color.resize(VERTICES_PER_OBJECT);
    for (int i = 0; i < VERTICES_PER_OBJECT; i++) {
        attribs.position[i] = cubeVertices.positions[i];
        attribs.color[i] = cubeVertices.colors[i];
    }

    //Convert Index Struct to Vector if Indices
    
    int NUMBER_OF_INDICES = 36; //36
    indices.resize(NUMBER_OF_INDICES);
    for (int i = 0; i < NUMBER_OF_INDICES; i++) {
        indices[i] = cubeIndices[i];
    }

    //Get size of new data structure
    int vertexCount = attribs.position.size();
    indices.size();

    std::vector<OVR::Vector3f> position;
    std::vector<OVR::Vector3f> normal;
    std::vector<OVR::Vector3f> tangent;
    std::vector<OVR::Vector3f> binormal;

    //GL Create the Vertex Buffer, IndexBuffer and VertexArrayObject
    

    //GL Generate Buffer Pointers
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &vertexArrayObject);
    
    //GL Bind to Buffers and Attribute Location
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindVertexArray(vertexArrayObject);
    glBindAttribLocation(program.ID, VERTEX_ATTRIBUTE_LOCATION_COLOR, "VertexColor");    
    
    //OVR Use the PackVertexAttribute function
    std::vector<uint8_t> packed;
    PackVertexAttribute(
        packed, attribs.position, VERTEX_ATTRIBUTE_LOCATION_POSITION, GL_FLOAT, 3);

    PackVertexAttribute(packed, attribs.color, VERTEX_ATTRIBUTE_LOCATION_COLOR, GL_FLOAT, 4);

    //GL Add Data to the Buffers
    glBufferData(GL_ARRAY_BUFFER, packed.size() * sizeof(packed[0]), packed.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(indices[0]),
        indices.data(),
        GL_STATIC_DRAW);

    //Unbind the Vertex Array
    glBindVertexArray(0);
    return 1;
}



void svrAppl::render() {}