#pragma once
#include <glm/glm.hpp>

#include "camera.h"
#include "chunk.h"

struct Plane {
  glm::vec3 point = {0.0f, 0.0f, 0.0f};
  glm::vec3 normal = {0.0f, 1.0f, 0.0f};
};

struct Frustum {
  Plane topFace;
  Plane bottomFace;
  Plane rightFace;
  Plane leftFace;
  Plane farFace;
  Plane nearFace;

  Frustum(const Camera& camera, float aspect, float fovY, float zNear,
          float zFar);
  bool isChunkInside(const ChunkPosition& position);
};
