#include "camera.h"

#include "app/constants.h"
#include "devices.h"

#include <simd/common.h>
#include <utility>

Camera::Camera(float sensitivity, float speed, std::vector<KeyCode> forwardKeys,
               std::vector<KeyCode> backwardKeys, std::vector<KeyCode> leftKeys,
               std::vector<KeyCode> rightKeys)
    : m_forwardKeys{forwardKeys}, m_backwardKeys{backwardKeys},
      m_leftKeys{leftKeys}, m_rightKeys{rightKeys}, m_sensitivity{sensitivity},
      m_speed{speed} {}

Camera::CameraVectors Camera::update() {
  bool forward{false};
  bool backward{false};
  bool left{false};
  bool right{false};

  for (auto key : m_forwardKeys)
    if (isKeyDown(key)) {
      forward = true;
      break;
    }
  for (auto key : m_backwardKeys)
    if (isKeyDown(key)) {
      backward = true;
      break;
    }
  for (auto key : m_leftKeys)
    if (isKeyDown(key)) {
      left = true;
      break;
    }
  for (auto key : m_rightKeys)
    if (isKeyDown(key)) {
      right = true;
      break;
    }

  auto mousePos{getMousePos()};

  float originX{AppConstants::WindowWidth / 2.0 + AppConstants::WindowX};
  float originY{getDisplaySize().second - AppConstants::WindowHeight / 2.0f -
                AppConstants::WindowY};

  auto viewMatrix{
      update(forward, backward, left, right, mousePos, originX, originY)};

  moveMouse(originX, originY);

  return viewMatrix;
}

Camera::CameraVectors Camera::update(bool forward, bool backward, bool left,
                                     bool right,
                                     std::pair<float, float> mousePos,
                                     float originX, float originY) {
  // update rotation
  m_yaw += (mousePos.first - originX) * m_sensitivity;
  m_pitch += (originY - mousePos.second) * m_sensitivity;

  if (m_yaw < 0)
    m_yaw += 360.0f;
  if (m_yaw > 360)
    m_yaw -= 360.0f;
  m_pitch = simd::clamp(m_pitch, -89.0, 89.0);

  float cosYaw{static_cast<float>(cos(m_yaw * M_PI / 180.0))};
  float sinYaw{static_cast<float>(sin(m_yaw * M_PI / 180.0))};
  float cosPitch{static_cast<float>(cos(m_pitch * M_PI / 180.0))};
  float sinPitch{static_cast<float>(sin(m_pitch * M_PI / 180.0))};

  simd::float3 forwardVector{cosYaw * cosPitch, sinPitch, sinYaw * cosPitch};
  simd::float3 upVector{0.0f, 1.0f, 0.0f};
  simd::float3 rightVector{
      simd::normalize(simd::cross(forwardVector, upVector))};
  upVector = simd::normalize(simd::cross(rightVector, forwardVector));

  if (forward) {
    m_x += m_speed * forwardVector[0];
    m_y += m_speed * forwardVector[1];
    m_z += m_speed * forwardVector[2];
  }
  if (backward) {
    m_x -= m_speed * forwardVector[0];
    m_y -= m_speed * forwardVector[1];
    m_z -= m_speed * forwardVector[2];
  }
  if (right) {
    m_x += m_speed * rightVector[0];
    m_y += m_speed * rightVector[1];
    m_z += m_speed * rightVector[2];
  }
  if (left) {
    m_x -= m_speed * rightVector[0];
    m_y -= m_speed * rightVector[1];
    m_z -= m_speed * rightVector[2];
  }

  simd::float3 pos{static_cast<float>(m_x), static_cast<float>(m_y),
                   static_cast<float>(m_z)};

  return CameraVectors{rightVector, upVector, forwardVector, pos};
}
