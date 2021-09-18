#ifndef DISABLE_MULTIVIEW
 #define DISABLE_MULTIVIEW 0
#endif
#define NUM_VIEWS 2
#define attribute in
#define varying out
#if defined( GL_OVR_multiview2 ) && ! DISABLE_MULTIVIEW
  #extension GL_OVR_multiview2 : require
  layout(num_views=NUM_VIEWS) in;
  #define VIEW_ID gl_ViewID_OVR
#else
  uniform lowp int ViewID;
  #define VIEW_ID ViewID
#endif

uniform highp mat4 ModelMatrix;

// Use a ubo in v300 path to workaround corruption issue on Adreno 420+v300
// when uniform array of matrices used.
uniform SceneMatrices
{
	highp mat4 ViewMatrix[NUM_VIEWS];
	highp mat4 ProjectionMatrix[NUM_VIEWS];
} sm;
// Use a function macro for TransformVertex to workaround an issue on exynos8890+Android-M driver:
// gl_ViewID_OVR outside of the vertex main block causes VIEW_ID to be 0 for every view.
// NOTE: Driver fix has landed with Android-N.
//highp vec4 TransformVertex( highp vec4 oPos )
//{
//	highp vec4 hPos = sm.ProjectionMatrix[VIEW_ID] * ( sm.ViewMatrix[VIEW_ID] * ( ModelMatrix * oPos ) );
//	return hPos;
//}
#define TransformVertex(localPos) (sm.ProjectionMatrix[VIEW_ID] * ( sm.ViewMatrix[VIEW_ID] * ( ModelMatrix * localPos )))