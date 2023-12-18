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

uniform mat4 ModelMatrix;
uniform vec3 lightDir;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

void main(void)
{
	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;

	exPosition = (ViewMatrix * ModelMatrix * vec4(inPosition, 1.0)).xyz;
	exTexcoord = inTexcoord;
	exNormal = mat3(transpose(inverse(ModelMatrix))) * inNormal;
	exEyeDir = normalize(-exPosition);
	rawNormal = inNormal;
	exLightDir = normalize(lightDir);

}
