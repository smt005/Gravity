#version 300 es
layout (location = 0) in vec3 aPos;

uniform mat4 uMatProjectionView;
uniform mat4 uMatViewModel;

void main()
{
    gl_Position = uMatProjectionView * uMatViewModel * vec4(aPos, 1.0);
	//gl_Position = uMatViewModel * vec4(aPos, 1.0);
}
