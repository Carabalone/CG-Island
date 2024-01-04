#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

uniform mat4 ModelMatrix;
uniform bool clip;
uniform vec4 clipPlane;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	if (clip) {
		gl_ClipDistance[0] = dot(ModelMatrix * vec4(inPosition, 1.0), vec4(0.0f, -1.0f, 0.0f, 0.0f));
	}

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}
