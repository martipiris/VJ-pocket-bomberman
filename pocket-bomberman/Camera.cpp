#include "Camera.h"
#include "LevelScene.h"
#include <algorithm>

Camera::Camera(int viewportWidth, int viewportHeight, const glm::ivec2& worldPixelSize, const glm::vec2& target)
    : m_target(target), m_position(0.f, 0.f), m_viewport(viewportWidth, viewportHeight), m_worldSize(worldPixelSize)
{
}

void Camera::update() {
    float halfW = m_viewport.x * 0.5f;
    float halfH = m_viewport.y * 0.5f;

    m_position.x = m_target.x - halfW;
    m_position.y = m_target.y - halfH;

    if (m_position.x < SCREEN_OFFSET_X) m_position.x = float(SCREEN_OFFSET_X);
    if (m_position.y < SCREEN_OFFSET_Y) m_position.y = float(SCREEN_OFFSET_Y);
    float maxX = float(m_worldSize.x) - float(m_viewport.x) + float(SCREEN_OFFSET_X);
    float maxY = float(m_worldSize.y) - float(m_viewport.y) + float(SCREEN_OFFSET_Y);
    if (m_position.x > maxX) m_position.x = std::max(maxX, 0.0f);
    if (m_position.y > maxY) m_position.y = std::max(maxY, 0.0f);
}

void Camera::setTarget(const glm::vec2& target) {
    m_target = target;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(-m_position.x, -m_position.y, 0.f));
}

glm::vec2 Camera::getPosition() const {
    return m_position;
}