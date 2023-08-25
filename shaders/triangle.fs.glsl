#version 450 core

in vec4 fragPosition;
out vec4 fColor;

uniform vec4 baseColor;

void main()
{
	fColor =  mix(baseColor, fragPosition, .4);
}
