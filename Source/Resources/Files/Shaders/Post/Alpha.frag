#version 330 core
//precision mediump float;
precision highp float;

uniform sampler2D sTexture;
uniform float uAlpha;

in vec2 vTexCoord;
out vec4 fragColor;

void main()
{
	vec4 color = texture2D(sTexture, vTexCoord);
	color -= uAlpha;
	fragColor = color;
}
