#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;
out vec3 exLightDir;
out vec3 exEyeDir;
out vec3 rawNormal;
out vec3 exLightPos;
out vec3 exPosWorld;
out vec3 exCameraPos;

uniform mat4 ModelMatrix;
uniform vec3 lightDir;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	vec3 testPos = inPosition;
	vec4 MCPosition = vec4(testPos, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

	exPosWorld = (ModelMatrix * vec4(testPos, 1.0)).xyz;
	exNormal = mat3(transpose(inverse(ModelMatrix))) * inNormal;
	exCameraPos = vec3(inverse(ViewMatrix)[3]);
}
