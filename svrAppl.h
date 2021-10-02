
//GLFW
#include <GLFW/glfw3.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class svrAppl {
    private:
        


    public:
        void init();
        int createWindow(unsigned int width, unsigned int height, char *name);
        void render();

        GLFWwindow* window;


};