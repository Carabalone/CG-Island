#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec4 exColor;

out vec4 FragmentColor;

void main(void)
{
    vec3 normalizedNormal = normalize(exNormal);
    vec3 modifiedColor = 0.5 * normalizedNormal;
    FragmentColor = exColor + vec4(modifiedColor, 1.0);
}

