#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uPrev;
uniform sampler2D uCurrent;
uniform float uDecay;
uniform float uOffset;

void main()
{
    vec4 prev = texture(uPrev, vUV) * uDecay - uOffset;
    vec4 curr = texture(uCurrent, vUV);
    FragColor = prev + curr;
}
