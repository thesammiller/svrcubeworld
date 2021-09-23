#version 330 core

in vec4 outColor;
out vec4 FragColor;

void main()
{
	//Why is my outColor not working?
	FragColor = outColor;
	//This works so I know that the compiler works...
	//FragColor = vec4(0.0f, 0.2f, 0.0f, 1.0f);
}


