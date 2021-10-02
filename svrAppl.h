//Needed for extensions
#define GL_GLEXT_PROTOTYPES
//GLFW
#include <GLFW/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

class svrAppl {
    private:
        


    public:
        svrAppl();
        void init();
        int createWindow(unsigned int width, unsigned int height, char *name);
        void render();

        GLFWwindow* window = 0;
        Shader program;


};