
in vec3 Position;
in vec4 VertexColor;
in mat4 VertexTransform;
out vec4 fragmentColor;
void main()
{
	gl_Position = sm.ProjectionMatrix[VIEW_ID] * ( sm.ViewMatrix[VIEW_ID] * ( VertexTransform * vec4( Position * 0.1, 1.0 ) ) );
	fragmentColor = VertexColor;
}