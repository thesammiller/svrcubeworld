//This file is an adaptation of the one in the OVR. I don't want to bring a lot of baggage with this logic.


enum class ovrProgramParmType : char {
    INT, // int
    INT_VECTOR2, // Vector2i
    INT_VECTOR3, // Vector3i
    INT_VECTOR4, // Vector4i
    FLOAT, // float
    FLOAT_VECTOR2, // Vector2f
    FLOAT_VECTOR3, // Vector3f
    FLOAT_VECTOR4, // Vector4f
    FLOAT_MATRIX4, // Matrix4f (always inverted for GL)
    TEXTURE_SAMPLED, // GlTexture
    BUFFER_UNIFORM, // read-only uniform buffer (GLSL: uniform)
    MAX
};


struct ovrProgramParm {
    const char* Name;
    ovrProgramParmType Type;
};


struct ovrUniform {
    // can be made as high as 16
    static const int MAX_UNIFORMS = 16;

    ovrUniform() : Location(-1), Binding(-1), Type(ovrProgramParmType::MAX) {}

    int Location; // the index of the uniform in the render program
    int Binding; // the resource binding (eg. texture image unit or uniform block)
    ovrProgramParmType Type; // the type of the data
};


struct ovrUniformData {
    ovrUniformData() : Data(nullptr), Count(1) {}

    void* Data; // pointer to data
    int Count; // number of items of ovrProgramParmType in the Data buffer
};

struct GlProgram {
    GlProgram()
        : Program(0),
          VertexShader(0),
          FragmentShader(0),
          Uniforms(),
          numTextureBindings(0),
          numUniformBufferBindings(0) {}

    static const int GLSL_PROGRAM_VERSION = 300; // Minimum requirement for multiview support.

    static GlProgram Build(
        const char* vertexSrc,
        const char* fragmentSrc,
        const ovrProgramParm* parms,
        const int numParms,
        const int programVersion = GLSL_PROGRAM_VERSION, // minimum requirement
        bool abortOnError = true);

    // Use this build variant for specifying extensions or other program directives.
    static GlProgram Build(
        const char* vertexDirectives,
        const char* vertexSrc,
        const char* fragmentDirectives,
        const char* fragmentSrc,
        const ovrProgramParm* parms,
        const int numParms,
        const int programVersion = GLSL_PROGRAM_VERSION, // minimum requirement
        bool abortOnError = true);

    static void Free(GlProgram& program);

    static void SetUseMultiview(const bool useMultiview_);

    bool IsValid() const {
        return Program != 0;
    }

    static const int MAX_VIEWS = 2;
    static const int SCENE_MATRICES_UBO_SIZE = 2 * sizeof(OVR::Matrix4f) * MAX_VIEWS;

    unsigned int Program;
    unsigned int VertexShader;
    unsigned int FragmentShader;

    // Globally-defined system level uniforms.
    ovrUniform ViewID; // uniform for ViewID; is -1 if OVR_multiview unavailable or disabled
    ovrUniform ModelMatrix; // uniform for "uniform mat4 ModelMatrix;"
    ovrUniform SceneMatrices; // uniform for "SceneMatrices" ubo :
                              // uniform SceneMatrices {
                              //   mat4 ViewMatrix[NUM_VIEWS];
                              //   mat4 ProjectionMatrix[NUM_VIEWS];
                              // } sm;

    ovrUniform Uniforms[ovrUniform::MAX_UNIFORMS];
    int numTextureBindings;
    int numUniformBufferBindings;
#if OVR_USE_UNIFORM_NAMES
    std::string UniformNames[ovrUniform::MAX_UNIFORMS];
#endif /// OVR_USE_UNIFORM_NAMES

    class MultiViewScope {
       public:
        MultiViewScope(bool enableMultView);
        ~MultiViewScope();

       private:
        bool wasEnabled;
    };
};
