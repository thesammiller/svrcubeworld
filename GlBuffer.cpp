#include "GlBuffer.h"

GlBuffer::GlBuffer() : target(0), buffer(0), size(0) {}

bool GlBuffer::Create(const GlBufferType_t type, const size_t dataSize, const void* data) {

    target = ((type == GLBUFFER_TYPE_UNIFORM) ? GL_UNIFORM_BUFFER : 0);
    size = dataSize;

    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(target, 0);

    return true;
}