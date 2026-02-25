#version 300 es
layout (location = 0) in vec3 aPos;

uniform mat4 uMatProjectionView;

void main()
{
    gl_Position = uMatProjectionView * vec4(aPos, 1.0);
	//gl_Position = vec4(aPos, 1.0);
}