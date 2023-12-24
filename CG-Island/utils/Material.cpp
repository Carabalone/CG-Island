#include "Material.h"

void sendMaterials(GLuint shaderProgram, Material* material) {

    GLuint colorLoc = glGetUniformLocation(shaderProgram, "material.color");
    GLuint ambientColorLoc = glGetUniformLocation(shaderProgram, "material.ambientColor");
    GLuint lightColorLoc = glGetUniformLocation(shaderProgram, "material.lightColor");
    GLuint specColorLoc = glGetUniformLocation(shaderProgram, "material.specColor");
    GLuint glossinessLoc = glGetUniformLocation(shaderProgram, "material.glossiness");

    glUniform3fv(colorLoc, 1, glm::value_ptr(material->color));
    glUniform3fv(ambientColorLoc, 1, glm::value_ptr(material->ambientColor));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(material->lightColor));
    glUniform3fv(specColorLoc, 1, glm::value_ptr(material->specColor));
    glUniform1f(glossinessLoc, material->glossiness);
}