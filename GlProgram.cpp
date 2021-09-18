#include "GlProgram.h"

#include <stdlib.h>

#include "shader_s.h"

#include <iostream>

static bool UseMultiview = false;



GlProgram::MultiViewScope::MultiViewScope(bool enableMultView) {
    wasEnabled = UseMultiview;
    GlProgram::SetUseMultiview(enableMultView);
}
GlProgram::MultiViewScope::~MultiViewScope() {
    GlProgram::SetUseMultiview(wasEnabled);
}

void GlProgram::SetUseMultiview(const bool useMultiview_) {
    UseMultiview = useMultiview_;
}


static GLuint
CompileShader(GLenum shaderType, const char* directives, const char* src, GLint programVersion) {
  

    //const char* postVersion = FindShaderVersionEnd(src);

    Shader shaderHeaders = Shader("vertexHeader.vs", "fragmentHeader.fs");
    
    std::string srcString;
    srcString = std::string("#version ") + std::to_string(programVersion) + std::string(" es\n");

    if (directives != NULL) {
        srcString.append(directives);
    }

    // TODO: If a c string isn't passed here, the previous contents of srcString (ie version info)
    // are corrupted. Determine why.
    srcString += std::string("#define DISABLE_MULTIVIEW ") + std::to_string(UseMultiview ? 0 : 1) +
        std::string("\n");

    if (shaderType == GL_VERTEX_SHADER) {
        srcString.append(shaderHeaders.vShaderCode);
    } else if (shaderType == GL_FRAGMENT_SHADER) {
        srcString.append(shaderHeaders.fShaderCode);
    }

    // The original got this more dynamically, but we can shortcut it a little bit in this implementation
    srcString.append("#version 330 core");

    src = srcString.c_str();

    GLuint shader = glCreateShader(shaderType);

    const int numSources = 1;
    const char* srcs[1];
    srcs[0] = src;

    glShaderSource(shader, numSources, srcs, 0);
    glCompileShader(shader);

    GLint r;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &r);
    if (r == GL_FALSE) {
        std::cout << "Compiling shader failed" << std::endl;
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
        return 0;
    }
    return shader;
}

GlProgram GlProgram::Build(
    const char* vertexSrc,
    const char* fragmentSrc,
    const ovrProgramParm* parms,
    const int numParms,
    const int requestedProgramVersion,
    bool abortOnError) {
    return Build(
        NULL, vertexSrc, NULL, fragmentSrc, parms, numParms, requestedProgramVersion, abortOnError);
}

GlProgram GlProgram::Build(
    const char* vertexDirectives,
    const char* vertexSrc,
    const char* fragmentDirectives,
    const char* fragmentSrc,
    const ovrProgramParm* parms,
    const int numParms,
    const int requestedProgramVersion,
    bool abortOnError) 
    {
            GlProgram p;

            //--------------------------
            // Compile and Create the Program
            //--------------------------

            int programVersion = requestedProgramVersion;
            if (programVersion < GLSL_PROGRAM_VERSION) {
                std::cout << "GlProgram: Program GLSL version requested, but does not meet required minimum" << std::endl;
            }

            p.VertexShader = CompileShader(GL_VERTEX_SHADER, vertexDirectives, vertexSrc, programVersion);
            if (p.VertexShader == 0) {
                Free(p);
                std::cout << 
                    "GlProgram: CompileShader GL_VERTEX_SHADER program failed" << std::endl;
                if (abortOnError) {
                    std::cout << "Failed to compile vertex shader" << std::endl;
                }
                return GlProgram();
            }

            p.FragmentShader =
                CompileShader(GL_FRAGMENT_SHADER, fragmentDirectives, fragmentSrc, programVersion);
            if (p.FragmentShader == 0) {
                Free(p);
                std::cout <<
                    "GlProgram: CompileShader GL_FRAGMENT_SHADER program failed" << std::endl;
                if (abortOnError) {
                    std::cout << "Failed to compile fragment shader" << std::endl;
                }
                return GlProgram();
            }

            p.Program = glCreateProgram();
            glAttachShader(p.Program, p.VertexShader);
            glAttachShader(p.Program, p.FragmentShader);

            //--------------------------
            // Set attributes before linking
            //--------------------------

            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_POSITION, "Position");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_NORMAL, "Normal");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_TANGENT, "Tangent");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_BINORMAL, "Binormal");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_COLOR, "VertexColor");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_UV0, "TexCoord");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_UV1, "TexCoord1");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_JOINT_INDICES, "JointIndices");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_JOINT_WEIGHTS, "JointWeights");
            glBindAttribLocation(p.Program, VERTEX_ATTRIBUTE_LOCATION_FONT_PARMS, "FontParms");

            //--------------------------
            // Link Program
            //--------------------------

            glLinkProgram(p.Program);

            GLint linkStatus;
            glGetProgramiv(p.Program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus == GL_FALSE) {
                GLchar msg[1024];
                glGetProgramInfoLog(p.Program, sizeof(msg), 0, msg);
                Free(p);
                std::cout << "GlProgram: Linking program failed" << std::endl;
                if (abortOnError) {
                    std::cout << "Failed to link program" << std::endl;
                }
                return GlProgram();
            }

            //--------------------------
            // Determine Uniform Parm Location and Binding.
            //--------------------------

            p.numTextureBindings = 0;
            p.numUniformBufferBindings = 0;

            // Globally-defined system level uniforms.
            {
                p.ViewID.Type = ovrProgramParmType::INT;
                p.ViewID.Location = glGetUniformLocation(p.Program, "ViewID");
                p.ViewID.Binding = p.ViewID.Location;

                p.SceneMatrices.Type = ovrProgramParmType::BUFFER_UNIFORM;
                p.SceneMatrices.Location = glGetUniformBlockIndex(p.Program, "SceneMatrices");
                if (p.SceneMatrices.Location >= 0) // this won't be present for v100 shaders.
                {
                    p.SceneMatrices.Binding = p.numUniformBufferBindings++;
                    glUniformBlockBinding(p.Program, p.SceneMatrices.Location, p.SceneMatrices.Binding);
                }

                p.ModelMatrix.Type = ovrProgramParmType::FLOAT_MATRIX4;
                p.ModelMatrix.Location = glGetUniformLocation(p.Program, "ModelMatrix");
                p.ModelMatrix.Binding = p.ModelMatrix.Location;
            }

            glUseProgram(p.Program);

            // Since this is just an error check, we're going to pass it for now
            for (int i = 0; i < numParms; ++i) {
            //    OVR_ASSERT(parms[i].Type != ovrProgramParmType::MAX);

                /// Trace the names of the uniform for this progam
        #if OVR_USE_UNIFORM_NAMES
                p.UniformNames[i] = std::string(parms[i].Name);
                ALOG(
                    "   GlProgram[ %d ]: Uniforms[%d] = `%s` %s",
                    p.Program,
                    i,
                    parms[i].Name,
                    parms[i].Type == ovrProgramParmType::TEXTURE_SAMPLED ? "Texture" : "");
        #endif /// OVR_USE_UNIFORM_NAMES

                p.Uniforms[i].Type = parms[i].Type;
                if (parms[i].Type == ovrProgramParmType::TEXTURE_SAMPLED) {
                    p.Uniforms[i].Location =
                        static_cast<int16_t>(glGetUniformLocation(p.Program, parms[i].Name));
                    p.Uniforms[i].Binding = p.numTextureBindings++;
                    glUniform1i(p.Uniforms[i].Location, p.Uniforms[i].Binding);
                } else if (parms[i].Type == ovrProgramParmType::BUFFER_UNIFORM) {
                    p.Uniforms[i].Location = glGetUniformBlockIndex(p.Program, parms[i].Name);
                    p.Uniforms[i].Binding = p.numUniformBufferBindings++;
                    glUniformBlockBinding(p.Program, p.Uniforms[i].Location, p.Uniforms[i].Binding);
                } else {
                    p.Uniforms[i].Location =
                        static_cast<int16_t>(glGetUniformLocation(p.Program, parms[i].Name));
                    p.Uniforms[i].Binding = p.Uniforms[i].Location;
                }
                if (false == (p.Uniforms[i].Location >= 0 && p.Uniforms[i].Binding >= 0)) {
        #if OVR_USE_UNIFORM_NAMES
                    p.UniformNames[i] = std::string(parms[i].Name);
                    ALOGW(
                        "   GlProgram[ %d ]: Uniforms[%d] = `%s` %s NOT BOUND / USED",
                        p.Program,
                        i,
                        parms[i].Name,
                        parms[i].Type == ovrProgramParmType::TEXTURE_SAMPLED ? "Texture" : "");
        #else
                    std::cout << "   GlProgram : Uniforms NOT BOUND / USED" << std::endl;
        #endif /// OVR_USE_UNIFORM_NAMES
                }

                //OVR_ASSERT(p.Uniforms[i].Location >= 0 && p.Uniforms[i].Binding >= 0);
            }

            glUseProgram(0);

            return p;
        }


void GlProgram::Free(GlProgram& prog) {
    glUseProgram(0);
    if (prog.Program != 0) {
        glDeleteProgram(prog.Program);
    }
    if (prog.VertexShader != 0) {
        glDeleteShader(prog.VertexShader);
    }
    if (prog.FragmentShader != 0) {
        glDeleteShader(prog.FragmentShader);
    }
    prog.Program = 0;
    prog.VertexShader = 0;
    prog.FragmentShader = 0;
}