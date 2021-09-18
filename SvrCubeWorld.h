#include "Appl.h"
#include <iostream>

#include "shader_s.h"

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
        // Called once per frame when the VR session is active.
        virtual OVRFW::ovrApplFrameOut AppFrame(const OVRFW::ovrApplFrameIn& in) override;
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

        GlProgram Program;
        GLint VertexTransformAttribute;

        
        GlGeometry Cube;

        unsigned int Random;

        ovrMatrix4f CenterEyeViewMatrix;

        float RandomFloat();

        
};

