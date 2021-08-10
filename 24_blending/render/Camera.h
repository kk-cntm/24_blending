#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

namespace render {

class Camera {
public:
    enum Type {
        fly, fps
    };
    
    struct CameraArgs {
        Type type;
        glm::vec3 position;
        glm::vec3 direction;
        float yaw;
        float pitch;
        float deltaTime;
    };

private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    float m_yaw;
    float m_pitch;
    float m_deltaTime;
    Type m_type;
    
    static const glm::vec3 s_up;
    static constexpr float s_moveNormalizer = 2.5f;
    
    void CalculateDirection();
    
public:
    Camera(CameraArgs args);
    void Set(Shader& shader);
    void AddYaw(float yaw);
    void AddPitch(float pitch);
    void SetDeltaTime(float deltaTime);
    void MoveForward();
    void MoveBackward();
    void MoveRight();
    void MoveLeft();
    void MoveForwardLeft();
    void MoveForwardRight();
    void MoveBackwardLeft();
    void MoveBackwardRight();
    glm::mat4 GetView();
    glm::vec3 GetPosition();
    glm::vec3 GetDirection();
};

}
