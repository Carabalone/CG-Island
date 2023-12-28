#version 330 core

in vec2 exTexcoord;

out vec4 FragmentColor;

uniform sampler2D depthMap;

const float near = 0.5;
const float far = 55.0;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));	
}

void main(void)
{
    FragmentColor = vec4(vec3(texture(depthMap, exTexcoord).r), 1.0);
}