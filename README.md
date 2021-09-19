# Oculus Mobile SDK Cloud XR Server 

TODO:
- Need to check the license for GLFW
- Need to check the license for LearnOpenGL
- Need to check the license for Oculus Mobile SDK

- Create folders for the different helper classes so that it's better organized
- Add the Appl::Init logic from Appl.cpp --> Framebuffers (maybe don't have to do this yet)
- Maybe create Appl::Init with the LearnOpenGL logic so that it just renders to one window
- Make sure that it can be controlled by Mouse (which can later be injected with data from the Quest)
- Activate all the controller features 
- Make it so that things render as TWO EYES (search for NUM_EYES) 


NOTES:
- Clear Color in Oculus SDK is unclear -- uses either SurfaceRendererAppl or Appl
- GlGeometry opens up a whole can of worms... What do I really need from there?