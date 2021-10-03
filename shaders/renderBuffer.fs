#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

//gets initially set to 0
uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, TexCoords);
}