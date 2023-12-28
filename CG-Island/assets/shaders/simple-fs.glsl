#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

out vec4 FragmentColor;

uniform sampler2D depthMap;

const float near = 0.1;
const float far = 100.0;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));	
}

void main(void)
{
    vec3 normalizedNormal = normalize(exNormal);
    //FragmentColor = vec4(texture(depthMap, exTexcoord).xyz, 1.0);
    FragmentColor = vec4(vec3(texture(depthMap, exTexcoord).x), 1.0);

    //FragmentColor = vec4(vec3(0.0f, 0.0f, 1.0f), 1.0f);
}
