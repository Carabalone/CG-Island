#version 330 core

in vec2 inPosition;
in vec2 inTexcoord;

out vec2 exTexcoord;


void main(void)
{
	exTexcoord = inTexcoord;

	//gl_Position = vec4(inPosition, 0.0f, 1.0f);
	gl_Position = vec4(vec3(inPosition.x - 0.5, inPosition.y - 0.5, 0.0f), 1.0f);
}
