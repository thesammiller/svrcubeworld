#include "GlGeometry.h"

static bool enableGeometryTransfom = false;
static OVR::Matrix4f geometryTransfom = OVR::Matrix4f();

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

void GlGeometry::Create(const VertexAttribs& attribs, const std::vector<TriangleIndex>& indices) {
    vertexCount = attribs.position.size();
    indexCount = indices.size();

    const bool t = enableGeometryTransfom;

    std::vector<OVR::Vector3f> position;
    std::vector<OVR::Vector3f> normal;
    std::vector<OVR::Vector3f> tangent;
    std::vector<OVR::Vector3f> binormal;

    /// we asked for incoming transfom
    if (t) {
        position.resize(attribs.position.size());
        normal.resize(attribs.normal.size());
        tangent.resize(attribs.position.size());
        binormal.resize(attribs.binormal.size());

        /// poor man's 3x3
        OVR::Matrix4f nt = geometryTransfom.Transposed();

        /// Positions use 4x4
        for (size_t i = 0; i < attribs.position.size(); ++i) {
            position[i] = geometryTransfom.Transform(attribs.position[i]);
        }
        /// TBN use 3x3
        for (size_t i = 0; i < attribs.normal.size(); ++i) {
            normal[i] = nt.Transform(attribs.normal[i]).Normalized();
        }
        for (size_t i = 0; i < attribs.tangent.size(); ++i) {
            tangent[i] = nt.Transform(attribs.tangent[i]).Normalized();
        }
        for (size_t i = 0; i < attribs.binormal.size(); ++i) {
            binormal[i] = nt.Transform(attribs.binormal[i]).Normalized();
        }
    }

    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    std::vector<uint8_t> packed;
    PackVertexAttribute(
        packed, t ? position : attribs.position, VERTEX_ATTRIBUTE_LOCATION_POSITION, GL_FLOAT, 3);
    PackVertexAttribute(
        packed, t ? normal : attribs.normal, VERTEX_ATTRIBUTE_LOCATION_NORMAL, GL_FLOAT, 3);
    PackVertexAttribute(
        packed, t ? tangent : attribs.tangent, VERTEX_ATTRIBUTE_LOCATION_TANGENT, GL_FLOAT, 3);
    PackVertexAttribute(
        packed, t ? binormal : attribs.binormal, VERTEX_ATTRIBUTE_LOCATION_BINORMAL, GL_FLOAT, 3);
    PackVertexAttribute(packed, attribs.color, VERTEX_ATTRIBUTE_LOCATION_COLOR, GL_FLOAT, 4);
    PackVertexAttribute(packed, attribs.uv0, VERTEX_ATTRIBUTE_LOCATION_UV0, GL_FLOAT, 2);
    PackVertexAttribute(packed, attribs.uv1, VERTEX_ATTRIBUTE_LOCATION_UV1, GL_FLOAT, 2);
    PackVertexAttribute(
        packed, attribs.jointIndices, VERTEX_ATTRIBUTE_LOCATION_JOINT_INDICES, GL_INT, 4);
    PackVertexAttribute(
        packed, attribs.jointWeights, VERTEX_ATTRIBUTE_LOCATION_JOINT_WEIGHTS, GL_FLOAT, 4);
    // clang-format off

    glBufferData(GL_ARRAY_BUFFER, packed.size() * sizeof(packed[0]), packed.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(indices[0]),
        indices.data(),
        GL_STATIC_DRAW);

    glBindVertexArray(0);

    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_POSITION);
    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_NORMAL);
    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_TANGENT);
    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_BINORMAL);
    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_COLOR);
    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_UV0);
    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_UV1);
    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_JOINT_INDICES);
    glDisableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION_JOINT_WEIGHTS);

    localBounds.Clear();
    for (int i = 0; i < vertexCount; i++) {
        localBounds.AddPoint(attribs.position[i]);
    }
}
