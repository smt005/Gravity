#version 330 core
precision mediump float;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
out vec2 vTexCoord;

uniform mat4 uMatProjectionView;
uniform mat4 uMatViewModel;
uniform vec3 uCameraPos;
uniform vec3 uPos;

void main()
{
    vTexCoord = aTexCoord;
	gl_Position = uMatProjectionView * uMatViewModel * vec4(aPosition, 1.0);
}
