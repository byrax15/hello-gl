#version 450 core

layout (location = 0) out vec4 fColor;

uniform vec4 triangleColor;

void main()
{
	fColor = triangleColor;
}
