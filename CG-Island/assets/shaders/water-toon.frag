#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

out vec4 FragmentColor;

void main(void)
{
	FragmentColor = vec4(vec3(0.678, 0.847, 0.902), 0.7f);
}

