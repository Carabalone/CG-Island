#version 330 core

in vec3 exPosition;
in vec2 exTexcoord;
in vec3 exNormal;

uniform Camera {
   mat4 ViewMatrix;
   mat4 ProjectionMatrix;
};

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
    // dont need to do anything because we dont have a color buffer
    gl_FragDepth = linearizeDepth(gl_FragCoord.z) / far;
}