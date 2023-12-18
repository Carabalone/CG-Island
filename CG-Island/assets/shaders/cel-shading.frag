#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;
//in vec4 exColor;
in vec3 exLightDir;
in vec3 exEyeDir;
in vec3 rawNormal;

out vec4 FragmentColor;
uniform vec3 lightDir;
uniform vec3 lineColor;
const vec3 lightPos = vec3(2.0, 2.0, 2.0);

void main(void)
{
    vec3 ambientColor = vec3(0.4f, 0.4f, 0.4f);
    vec3 color = vec3(0.6, 0.1, 0.1);
    vec3 lightColor = vec3(1.0);

    vec3 normal = normalize(exNormal);
    vec3 eyeDir = normalize(-exPosition);

    float glossiness = 32.0;
    vec3 specColor = vec3(0.9);
    vec3 normalizedexLightDir = normalize(lightPos - exPosition);


    float NdotL = dot(normal, normalizedexLightDir);
    float diff = max(NdotL, 0.0f);
    float flatShade = step(0.5, diff);
    float intensity = smoothstep(0, 0.05, diff);
    vec3 light = lightColor * intensity;

    vec3 halfVec = normalize(normalizedexLightDir + eyeDir);
    float specAngle = max(dot(halfVec, normal), 0.0f);
    float specIntensity = pow(specAngle, glossiness * 16);
    specIntensity = smoothstep(0, 0.01, specIntensity);
    vec3 specular = specIntensity * specColor;

    vec3 rimColor = vec3(1.0);
    float rimAmmount = 0.716;
    float rimThreshold = 0.2;

    float rim = NdotL > 0 ? 1 - max(dot(eyeDir, normal), 0.0f) : 0.0f;
    vec3 rimLight = vec3(rim);
    rimLight = vec3(smoothstep(0.9, 1.0, rim));

    vec3 finalColor = color * (ambientColor + light + specular);

    //FragmentColor = vec4(specular.r > 0.5 ? specular : vec3(0.2), 1.0);
    FragmentColor = vec4(finalColor, 1.0);
    //FragmentColor = vec4(normalizedexLightDir, 1.0f);
    //FragmentColor = vec4(exEyeDir, 1.0f);
    //FragmentColor = vec4(halfVec, 1.0f);
    //FragmentColor = vec4(vec3(dot(normal, eyeDir)), 1.0f);
    //FragmentColor = vec4(normal, 1.0f);
    //FragmentColor = vec4(rawNormal, 1.0);

}
