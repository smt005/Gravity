#version 300 es
precision mediump float;

uniform sampler2D sTexture;

in vec2 vTexCoord;
out vec4 fragColor;

void main()
{
    vec4 color = texture2D(sTexture, vTexCoord);
	fragColor = color;
}
