#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

uniform sampler2D tex1;
uniform float resolution;

out vec4 FragmentColor;

uniform sampler2D normalMap;

// Light properties
vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
vec3 lightColor = vec3(0.0, 1.0, 1.0);
float ambientStrength = 0.2;

void main(void)
{

	vec3 test = texture(normalMap, exTexcoord).rgb;

    vec3 normal = normalize(exNormal);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;

    vec3 result = ambient + diffuse;

    // Output final color

    FragmentColor = vec4(result * vec3(0.635, 0.909, 0.909), 0.7);

}

