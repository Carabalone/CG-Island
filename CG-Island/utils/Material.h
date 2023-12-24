#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Material {
    glm::vec3 color;
    glm::vec3 ambientColor;
    glm::vec3 lightColor;
    glm::vec3 specColor;
    float glossiness;

    Material(const glm::vec3& _color, const glm::vec3& _ambientColor,
        const glm::vec3& _lightColor, const glm::vec3& _specColor, float _glossiness)
        : color(_color), ambientColor(_ambientColor),
        lightColor(_lightColor), specColor(_specColor), glossiness(_glossiness) {}
    
};

void sendMaterials(GLuint shaderProgram, Material* material);

#endif // MATERIAL_H
