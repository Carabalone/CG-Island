#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec4 viewSpacePos;

uniform sampler2D waveFoamNoise;
uniform sampler2D depthMap;
uniform sampler2D dudvMap;
uniform sampler2D refractedTexture;

uniform float resolution;
uniform float time;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

out vec4 FragmentColor;

// Light properties
vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
vec3 lightColor = vec3(0.0, 1.0, 1.0);
float ambientStrength = 0.2;

const float foamDistance = 0.015f;

const float depthMax = 1.0;
const vec4 shallowColor = vec4(0.1f, 0.95, 0.95, 0.78f);
const vec4 deepColor = vec4(0.086, 0.1, 1.0f, 1.0f);

const float waveStrength = 0.02f;

float getNearPlane(mat4 projectionMatrix) {
    return projectionMatrix[2][3] / (projectionMatrix[2][2] - 1.0);
}

float getFarPlane(mat4 projectionMatrix) {
    return projectionMatrix[2][3] / (projectionMatrix[2][2] + 1.0);
}

float near = getNearPlane(ProjectionMatrix);
float far = getFarPlane(ProjectionMatrix);


float linearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main(void)
{
    vec2 ndc = vec2(gl_FragCoord.x / 800, gl_FragCoord.y / 600);
    vec2 distortion = (texture(dudvMap, exTexcoord).rg * 2);
    vec2 smallFoamNoise = (texture(dudvMap, exTexcoord * 5.0f + 0.01 * time).rg * 2);
    vec2 smallerFoamNoise = (texture(dudvMap, exTexcoord * 8.0f + 0.01 * time).rg * 2);

    vec2 refractedDistortion = texture(dudvMap, exTexcoord + time * 0.02f).rg * waveStrength;
    vec2 refractCoords = clamp(ndc + refractedDistortion, 0.001, 0.999); // clamp to avoid texture wrapp because of offset

	vec3 normal = normalize(exNormal);

    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diffuseStrength * lightColor;

    vec3 pos = viewSpacePos.xyz;

    float specularStrength = pow(max(dot(normal, normalize(pos)), 0.0), 32);
    vec3 specularColor = vec3(1.0, 1.0, 1.0);
	vec3 specular = specularStrength * specularColor;

    vec3 lighting = (ambientStrength + diffuse + specular) * vec3(1.0f);

    float depth = texture(depthMap, ndc).r;
    
    float waterDiff = clamp((depth - linearizeDepth(gl_FragCoord.z) / far), 0.0, 1.0);
    vec4 waterColor = mix(shallowColor, deepColor, clamp(waterDiff * 3.0f, 0.0, 1.0f));
    waterColor = waterColor * vec4(lighting, 1.0f);

    vec2 surfaceFoamCoords = vec2(exTexcoord.x * 0.8f, exTexcoord.y * 0.5f) + distortion * 0.08f;
    float surfaceFoamCostline = texture(waveFoamNoise, exTexcoord * 10.0f + time * 0.045f).r;
    float surfaceFoam = texture(waveFoamNoise, surfaceFoamCoords + time * 0.01f).r;


    float foamDepthDifference = clamp(waterDiff / foamDistance, 0.0, 1.0);
    float surfaceNoiseCutoff = foamDepthDifference * 0.777;


    float coastlineNoise = surfaceFoamCostline > surfaceNoiseCutoff ? 1 : 0;
    float surfaceNoise = surfaceFoam > 0.7 ? 1 : 0;

    surfaceNoise += coastlineNoise + smallFoamNoise.r * 0.02f + smallerFoamNoise.r * 0.011f;
    surfaceNoise = clamp(surfaceNoise, 0.0, 1.0);

    vec4 finalColor = mix(
			vec4(waterColor.xyz + vec3(surfaceNoise), 1.0f),
			vec4(texture(refractedTexture, refractCoords).rgba),
			1.0 - waterColor.w
    );

    //FragmentColor = vec4(waterColor.xyz + vec3(surfaceNoise), waterColor.w);
    //FragmentColor = vec4(vec3(waterDiff), 1.0f);
    //FragmentColor = vec4(waterColor);
    //FragmentColor = vec4(texture(refractedTexture, ndc).rgb, 1.0f);
    FragmentColor = finalColor;

}

