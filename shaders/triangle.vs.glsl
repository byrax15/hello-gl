#version 450 core

in vec3 vPosition;
out vec4 fragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 pos = vec4(vPosition.x, vPosition.y, vPosition.z, 1);
	gl_Position = projection * view * model * pos;
	fragPosition = pos;
}
