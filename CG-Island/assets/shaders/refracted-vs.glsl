#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;

uniform mat4 ModelMatrix;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

const height = 0.0;
vec4 clipPlane = vec4(0.0, -1.0, 0.0, height);

void main(void)
{
	exPosition = inPosition;
	exTexcoord = inTexcoord;
	exNormal = inNormal;

	gl_ClipDistance[0] = dot(ModelMatrix * vec4(inPosition, 1.0), clipPlane);

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

}
