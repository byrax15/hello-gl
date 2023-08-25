#version 450 core

in vec3 vPosition;
out vec4 fragPosition;

void main()
{
	vec4 pos = vec4(vPosition.x, vPosition.y, vPosition.z, 1);
	gl_Position = pos;
	fragPosition = pos;
}
