#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPos;

uniform mat4 uMatProjectionView;
uniform mat4 uMatViewModel;

void main()
{
    gl_Position = uMatProjectionView * uMatViewModel * vec4(aPos, 1.0);
}
