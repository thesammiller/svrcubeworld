
#include <GLFW/glfw3.h>

void GLCheckErrors(int line);


#define GL(func) \
    func;        \
    GLCheckErrors(__LINE__);
