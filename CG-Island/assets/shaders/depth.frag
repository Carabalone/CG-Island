#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

//out vec4 FragmentColor;

const float near = 0.1;
const float far = 100.0;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));	
}

void main(void)
{
    //vec3 normalizedNormal = normalize(exNormal);
    //FragmentColor = vec4(vec3(linearizeDepth(gl_FragCoord.z) / far - gl_FragCoord.w), 1.0);
    //gl_FragDepth = linearizeDepth(gl_FragCoord.z) / far - gl_FragCoord.w;
}