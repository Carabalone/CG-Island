#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exPosition;
out vec2 exTexcoord;
out vec3 exNormal;

uniform mat4 ModelMatrix;
uniform sampler2D tex1;
uniform float time;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

float height(vec2 pos, float time)
{
	float height = 0.0f;
	float frequency = 0.5f;
	float amplitude = 1.0f;
	float persistence = 0.5f;
	int octaves = 4;

	for (int i = 0; i < octaves; i++)
	{
		//height += amplitude * texture(tex1, pos * frequency).r;
		height += amplitude * texture(tex1, pos * frequency + vec2(time * 0.1, 0.0)).r;
		frequency *= 2.0f;
		amplitude *= persistence;
	}

	return height;
}	

void main(void)
{
	exPosition = inPosition;
	exTexcoord = inTexcoord;
	exNormal = inNormal;

	vec3 position = inPosition;
	float k = height(position.xz/128, time);
	position.y = k;

	exPosition = (ModelMatrix * vec4(position, 1.0f)).xyz;

	exNormal = normalize(vec3(position.y - height(position.xz + vec2(0.1, 0.0), time * 0.1), 0.1, k - height(position.xz + vec2(0.0, 0.1), time * 0.1)));

	vec4 MCPosition = vec4(position, 1.0);
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * MCPosition;
}
