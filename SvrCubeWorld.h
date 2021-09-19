#include "Appl.h"
#include <iostream>

#include "shader_s.h"

static const int NUM_INSTANCES = 1500;
static const int NUM_ROTATIONS = 16;

class SvrCubeWorld : public svrAppl {
    public:
        SvrCubeWorld();

        // Called when the application initializes.
        // Must return true if the application initializes successfully.
        bool AppInit() override;
        /*
        // Called when the application shuts down
        virtual void AppShutdown() override;
        // Called when the application is resumed by the system.
        virtual void AppResumed() override;
        // Called when the application is paused by the system.
        virtual void AppPaused() override;
        */
        // Called once per frame when the VR session is active.
        virtual svrApplFrameOut AppFrame(const svrApplFrameIn& in) override;
        /*
        // Called once per frame to allow the application to render eye buffers.
        virtual void AppRenderFrame(const OVRFW::ovrApplFrameIn& in, OVRFW::ovrRendererOutput& out)
            override;
        // Called once per eye each frame for default renderer
        virtual void
        AppRenderEye(const OVRFW::ovrApplFrameIn& in, OVRFW::ovrRendererOutput& out, int eye) override;
        */
    private:
        svrRenderState RenderState;
        svrSurfaceRender SurfaceRender;

        svrSurfaceDef SurfaceDef;
        //OpenGL Program with Shaders
        GlProgram Program;
        GLint VertexTransformAttribute;
        //OpenGl Vertex Attributes
        GlGeometry Cube;
        GLuint InstanceTransformBuffer;
        //Game Logic for Rotations and Positions
        ovrVector3f Rotations[NUM_ROTATIONS];
        ovrVector3f CubePositions[NUM_INSTANCES];  
        int CubeRotations[NUM_INSTANCES];
      
        ovrMatrix4f CenterEyeViewMatrix;
        unsigned int Random;
        float RandomFloat();

        double startTime;

        
};

