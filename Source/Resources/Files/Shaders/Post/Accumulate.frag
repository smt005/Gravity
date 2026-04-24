#version 330 core
precision highp float;

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uPrev;
uniform sampler2D uCurrent;
uniform float uDecay;

void main()
{
    vec4 prev = texture(uPrev, vUV) - uDecay;
    vec4 curr = texture(uCurrent, vUV);
	FragColor = prev + curr;
}
