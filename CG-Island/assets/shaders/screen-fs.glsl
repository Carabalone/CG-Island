#version 330 core

in vec2 exTexcoord;

out vec4 FragmentColor;

uniform sampler2D refractedTexture;

void main(void)
{
    FragmentColor = vec4(vec3(texture(refractedTexture, gl_FragCoord.xy / vec2(800, 600)).r), 1.0);
}