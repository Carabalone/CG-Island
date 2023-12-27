#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

out vec4 FragmentColor;

void main(void)
{
    vec3 normalizedNormal = normalize(exNormal);
    FragmentColor = vec4(modifiedColor, 1.0);
}
