#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec4 screenSpacePos;

uniform sampler2D tex1;
uniform sampler2D depthMap;
uniform float resolution;

out vec4 FragmentColor;

uniform sampler2D normalMap;

// Light properties
vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
vec3 lightColor = vec3(0.0, 1.0, 1.0);
float ambientStrength = 0.2;

const float near = 0.5;
const float far = 55.0;

const float depthMax = 1.0;
const vec4 shallowColor = vec4(0, 1, 0.8, 0.98);
const vec4 deepColor = vec4(0.1, 0.1, 0.4, 0.98);


float linearizeDepth(float depth)
{

    return 0.5*(-near*depth + far) / depth + 0.5;
	//return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));	
}

void main(void)
{

	vec3 test = texture(normalMap, exTexcoord).rgb;

    vec3 normal = normalize(exNormal);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;

    vec3 result = ambient + diffuse;

    vec2 texCoord2 = vec2(gl_FragCoord.x / 800, gl_FragCoord.y / 600);
    float depth = texture(depthMap, texCoord2).r;
    
    float waterDiff = clamp(depth / 0.9, 0.0, 1.0);

    vec4 waterColor = mix(shallowColor, deepColor, clamp(waterDiff, 0.0, 1.0f));

    // Output final color

    //FragmentColor = vec4(result * vec3(0.635, 0.909, 0.909), 0.7);
    FragmentColor = waterColor;
    //FragmentColor = vec4(vec3(depth), 1.0);

}

