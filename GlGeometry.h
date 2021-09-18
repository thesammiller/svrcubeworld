#include <vector>

#include "OVR_Math.h"
#include "GlProgram.h"

#pragma once

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

class GlGeometry {
   public:
    GlGeometry()
        : vertexBuffer(0),
          indexBuffer(0),
          vertexArrayObject(0),
          primitiveType(0x0004 /* GL_TRIANGLES */),
          vertexCount(0),
          indexCount(0),
          localBounds(OVR::Bounds3f::Init) {}

    GlGeometry(const VertexAttribs& attribs, const std::vector<TriangleIndex>& indices)
        : vertexBuffer(0),
          indexBuffer(0),
          vertexArrayObject(0),
          primitiveType(0x0004 /* GL_TRIANGLES */),
          vertexCount(0),
          indexCount(0),
          localBounds(OVR::Bounds3f::Init) {
        Create(attribs, indices);
    }

    // Create the VAO and vertex and index buffers from arrays of data.
    void Create(const VertexAttribs& attribs, const std::vector<TriangleIndex>& indices);
    void Update(const VertexAttribs& attribs, const bool updateBounds = true);

    // Free the buffers and VAO, assuming that they are strictly for this geometry.
    // We could save some overhead by packing an entire model into a single buffer, but
    // it would add more coupling to the structures.
    // This is not in the destructor to allow objects of this class to be passed by value.
    void Free();

   public:
    static constexpr int32_t MAX_GEOMETRY_VERTICES = 1 << (sizeof(TriangleIndex) * 8);
    static constexpr int32_t MAX_GEOMETRY_INDICES = 1024 * 1024 * 3;

    static constexpr inline int32_t GetMaxGeometryVertices() {
        return MAX_GEOMETRY_VERTICES;
    }
    static constexpr inline int32_t GetMaxGeometryIndices() {
        return MAX_GEOMETRY_INDICES;
    }

    static unsigned IndexType; // GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, etc.

    class TransformScope {
       public:
        TransformScope(const OVR::Matrix4f m, bool enableTransfom = true);
        ~TransformScope();

       private:
        OVR::Matrix4f previousTransform;
        bool wasEnabled;
    };

    struct Descriptor {
        Descriptor(
            const VertexAttribs& a,
            const std::vector<TriangleIndex>& i,
            const OVR::Matrix4f& t)
            : attribs(a), indices(i), transform(t) {}
        Descriptor() : transform(OVR::Matrix4f::Identity()) {}

        VertexAttribs attribs;
        std::vector<TriangleIndex> indices;
        OVR::Matrix4f transform;
    };

   public:
    unsigned vertexBuffer;
    unsigned indexBuffer;
    unsigned vertexArrayObject;
    unsigned primitiveType; // GL_TRIANGLES / GL_LINES / GL_POINTS / etc
    int vertexCount;
    int indexCount;
    OVR::Bounds3f localBounds;
};