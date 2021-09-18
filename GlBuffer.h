#include "Egl.h"


enum GlBufferType_t {
    GLBUFFER_TYPE_UNIFORM,
};

class GlBuffer {
   public:
    GlBuffer();

    bool Create(const GlBufferType_t type, const size_t dataSize, const void* data);
    void Destroy();

    void Update(const size_t updateDataSize, const void* data) const;

    void* MapBuffer() const;
    void UnmapBuffer() const;

    unsigned int GetBuffer() const {
        return buffer;
    }
    size_t GetSize() const {
        return size;
    }

   private:
    unsigned int target;
    unsigned int buffer;
    size_t size;
};