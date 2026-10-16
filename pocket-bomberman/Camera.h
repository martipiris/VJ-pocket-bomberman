#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"


class Camera {

public:
    Camera(int viewportWidth, int viewportHeight, const glm::ivec2& worldPixelSize, const glm::vec2& target);

    void update();
    void setTarget(const glm::vec2& target);

    glm::mat4 getViewMatrix() const;

    glm::vec2 getPosition() const;
    

private:
    glm::vec2 m_target;
    glm::vec2 m_position;
    glm::ivec2 m_viewport;
    glm::ivec2 m_worldSize;
};

#endif // _CAMERA_INCLUDE