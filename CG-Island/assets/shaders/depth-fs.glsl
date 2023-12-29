#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

const float near = 0.5;
const float far = 55.0;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));	
}

void main(void)
{
    // dont need to do anything because we dont have a color buffer
    gl_FragDepth = linearizeDepth(gl_FragCoord.z) / far;
}