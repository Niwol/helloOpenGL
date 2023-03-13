#pragma once

#include "fwd.hpp"
#include <glm/glm.hpp>

class Transform
{
public:
    Transform();
    Transform(const Transform& other);
    ~Transform() = default;

    void translate(glm::vec3 translation);
    void scale(glm::vec3 scaler);
    
    // Not preventing Gimbal lock
    void rotate(glm::vec3 axis, float theta);

    inline glm::mat4 getModel() { return m_model; }


    Transform& operator=(const Transform& other);

private:
    void updateModel();

private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::mat4 m_rotation;

    glm::mat4 m_model;
};
