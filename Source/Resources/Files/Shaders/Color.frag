#version 330 core
precision mediump float;

uniform vec4 uColor;

out vec4 FragColor;

void main()
{
    FragColor = uColor;
}
