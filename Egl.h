
#include <GLFW/glfw3.h>

#pragma once

void GLCheckErrors(int line);


#define GL(func) \
    func;        \
    GLCheckErrors(__LINE__);



