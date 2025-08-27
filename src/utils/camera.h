#pragma once

#include "devices.h"

#include <simd/simd.h>
#include <vector>

class Camera {
public:
  struct CameraVectors {
    simd::float3 right;
    simd::float3 up;
    simd::float3 forward;
    simd::float3 pos;
  };

  Camera(float sensitivity = 0.1, float speed = 0.5,
         std::vector<KeyCode> forwardKeys = {KeyCode::W},
         std::vector<KeyCode> backwardKeys = {KeyCode::S},
         std::vector<KeyCode> leftKeys = {KeyCode::A},
         std::vector<KeyCode> rightKeys = {KeyCode::D});

  // returns camera view matrix
  // Listens for keys, looks for mouse movements, recenters mouse
  CameraVectors update();

private:
  // Returns the view matrix
  CameraVectors update(bool forward, bool backward, bool left, bool right,
                       std::pair<float, float> mousePos, float originX,
                       float originY);

  double m_x, m_y, m_z, m_yaw, m_pitch;

  double m_sensitivity, m_speed;

  std::vector<KeyCode> m_forwardKeys, m_backwardKeys, m_leftKeys, m_rightKeys;
};
