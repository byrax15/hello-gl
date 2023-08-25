#version 450 core

in vec4 fragPosition;
out vec4 fColor;

void main()
{
	fColor = fragPosition;
}
