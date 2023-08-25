#version 450 core

out vec4 fColor;

uniform vec4 color;
uniform float time;

void main()
{
	fColor = color * time;
}
