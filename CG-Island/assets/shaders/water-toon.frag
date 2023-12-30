#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
in vec4 screenSpacePos;

uniform sampler2D waveFoamNoise;
uniform sampler2D depthMap;
uniform sampler2D surfaceDepth;
uniform sampler2D dudvMap;

uniform float resolution;
uniform float time;

out vec4 FragmentColor;

uniform sampler2D normalMap;

// Light properties
vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
vec3 lightColor = vec3(0.0, 1.0, 1.0);
float ambientStrength = 0.2;

const float foamDistance = 0.015f;

const float depthMax = 1.0;
const vec4 shallowColor = vec4(0, 1, 0.8, 0.78);
const vec4 deepColor = vec4(0.1, 0.1, 0.4, 0.78);

void main(void)
{

    vec2 texCoord2 = vec2(gl_FragCoord.x / 800, gl_FragCoord.y / 600);
    float depth = texture(depthMap, texCoord2).r;
    float surfaceDepth = texture(surfaceDepth, texCoord2).r;


    float waterSurfaceDepth = gl_FragCoord.y / 600;
    
    float waterDiff = clamp((depth - waterSurfaceDepth), 0.0, 1.0);
    waterDiff = clamp((depth - surfaceDepth) * 1.5f, 0.0, 1.0);

    vec4 waterColor = mix(shallowColor, deepColor, clamp(waterDiff, 0.0, 1.0f));

    float surfaceFoamCostline = texture(waveFoamNoise, exTexcoord * 10.0f + time * 0.1f).r;

    vec2 distortion = (texture(dudvMap, exTexcoord).rg * 2) * 0.08f;
    vec2 surfaceFoamCoords = vec2(exTexcoord.x * 0.8f, exTexcoord.y * 0.5f) + distortion;
    float surfaceFoam = texture(waveFoamNoise, surfaceFoamCoords + time * 0.01f).r;

    float foamDepthDifference = clamp(waterDiff / foamDistance, 0.0, 1.0);
    float surfaceNoiseCutoff = foamDepthDifference * 0.777;

    float coastlineNoise = surfaceFoamCostline > surfaceNoiseCutoff ? 1 : 0;

    float surfaceNoise = surfaceFoam > 0.7 ? 1 : 0;

    surfaceNoise += coastlineNoise;
    surfaceNoise = clamp(surfaceNoise, 0.0, 1.0);

    //FragmentColor = waterColor + surfaceNoise;
    FragmentColor = vec4(waterColor.xyz + vec3(surfaceNoise), waterColor.w);
    //FragmentColor = vec4(vec3(surfaceNoiseCutoff), 1.0f);
    //FragmentColor = vec4(vec3(surfaceFoam), 1.0f);
    //FragmentColor = vec4(vec3((gl_FragCoord.z / gl_FragCoord.w + 1.0f) / 0.5f), 1.0f);
    //FragmentColor = vec4(vec3(waterDiff), 1.0f);
    //FragmentColor = vec4(waterColor);
    //FragmentColor = vec4(vec3(waterDiff), 1.0f);
    //FragmentColor = vec4(vec3((depth + screenSpacePos.z)), 1.0f);
    //FragmentColor = vec4(texture(dudvMap, exTexcoord).rgb, 1.0f);

}

