#version 330 core


in vec3 vertexPosition;
in vec4 vertexColor;
in mat4 vertexTransform;

uniform mat4 projection;
uniform mat4 view;

out vec4 fragmentColor;

void main()
{
	//gl_Position = projection * ( view * ( vertexTransform * vec4( vertexPosition * 0.1, 1.0 ) ) );
	gl_Position = vec4(vertexPosition, 1.0);
	fragmentColor = vertexColor;
}