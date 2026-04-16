#version 300 es
//precision mediump float;
precision highp float;

//uniform sampler2D sTexture;

uniform float uAlpha;
uniform sampler2D uPrevTex;
uniform sampler2D uCurrTex;

in vec2 vTexCoord;
out vec4 fragColor;

void main()
{
    vec4 prevColor = texture2D(uPrevTex, vTexCoord);
	vec4 curColor = texture2D(uCurrTex, vTexCoord);
	prevColor *= uAlpha;
	vec4 color = min(prevColor + curColor, vec4(1.0));
	fragColor = color;
}
