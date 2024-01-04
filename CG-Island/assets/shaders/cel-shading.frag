#version 330 core

in vec3 exNormal;
in vec3 exLightDir;
in vec3 exPosWorld;
in vec3 exCameraPos;
in vec2 exTexCoord;

out vec4 FragmentColor;

uniform vec3 lightDir;
uniform vec3 lineColor;
uniform sampler2D tex1;
uniform sampler2D normalMap;

uniform bool useTexture;
uniform vec3 colorUniform;
uniform float glossiness;

const vec3 lightPos = vec3(-7.0f, 7.0f, -7.0f);
const vec3 ambientColor = vec3(0.2f, 0.2f, 0.2f);
const vec3 lightColor = vec3(1.0);
const vec3 specColor = vec3(1.0);

void main(void)
{
    vec3 normal = normalize(exNormal);
    vec3 eyeDir = normalize(exCameraPos-exPosWorld);
    vec3 color = colorUniform;

    if (useTexture) {
        vec3 normalMap = texture(normalMap, exTexCoord).rgb;
        //normal = normalize(normalMap);
    }

    vec3 normalizedexLightDir = normalize(lightPos - exPosWorld);

    float NdotL = dot(normal, normalizedexLightDir);
    float diff = max(NdotL, 0.0f);
    float intensity = smoothstep(0, 0.01, diff);
    vec3 light = lightColor * intensity;
    light = lightColor * intensity * 0.9;

    vec3 halfVec = normalize(normalizedexLightDir + eyeDir);
    float specAngle = max(dot(halfVec, normal), 0.0f);
    float specIntensity = pow(specAngle, glossiness * 16);
    specIntensity = smoothstep(0, 0.0015, specIntensity);
    vec3 specular = specIntensity * specColor * 1.0;

    if (useTexture) {
	    color = texture(tex1, exTexCoord).rgb;
    }
	vec3 finalColor = color * (ambientColor + light + specular);
//
//    FragmentColor = vec4(specular.r > 0.5 ? specular : vec3(0.2), 1.0);
//	FragmentColor = vec4(normalizedexLightDir, 1.0f);
    //FragmentColor = vec4(exPosition, 1.0f);
//    FragmentColor = vec4(exPosWorld, 1.0f);
//    FragmentColor = vec4(normalizedexLightDir, 1.0f);
//    //FragmentColor = vec4(exEyeDir, 1.0f);
    //FragmentColor = vec4(halfVec, 1.0f);
    //FragmentColor = vec4(vec3(dot(normal, eyeDir)), 1.0f);
    //FragmentColor = vec4(normal, 1.0f);
    //FragmentColor = vec4(rawNormal, 1.0);
    FragmentColor = vec4(finalColor, 1.0);
}
