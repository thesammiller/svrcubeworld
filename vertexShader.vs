#version 330 core


layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
in mat4 VertexTransform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec4 outColor;

void main()
{
	gl_Position = projection * view * VertexTransform * vec4(vertexPosition, 1.0);
	outColor = vertexColor;
}