#version 460 core

in vec3 fragmentColor;

out vec4 screenColor;

void main()
{
    screenColor = vec4(fragmentColor, 1.0);
	// screenColor = vec4(1.0f, 0, 0, 1.0);
}