#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec2 exTexCoord;
out vec3 exNormal;
out vec3 exPosWorld;
out vec3 exCameraPos;

uniform mat4 ModelMatrix;
uniform vec3 lightDir;
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

	vec3 testPos = inPosition;
	vec4 MCPosition = vec4(testPos, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

	exPosWorld = (ModelMatrix * vec4(testPos, 1.0)).xyz;
	exNormal = mat3(transpose(inverse(ModelMatrix))) * inNormal;
	exCameraPos = vec3(inverse(ViewMatrix)[3]);
	exTexCoord = inTexcoord;
}
