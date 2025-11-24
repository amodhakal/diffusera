#include "config.h"
#include "frustum.h"

Frustum::Frustum(const Camera& camera, float aspect, float fovY, float zNear,
                 float zFar) {
  float halfVSide = zFar * tanf(fovY * 0.5);
  float halfHSide = halfVSide * aspect;
  const glm::vec3 frontMultFar = zFar * camera.m_Front;

  nearFace = {camera.m_Position + zNear * camera.m_Front, camera.m_Front};
  farFace = {camera.m_Position + frontMultFar, -camera.m_Front};
  rightFace = {
      camera.m_Position,
      glm::cross(frontMultFar - camera.getRight() * halfHSide, camera.m_Up)};
  leftFace = {
      camera.m_Position,
      glm::cross(camera.m_Up, frontMultFar + camera.getRight() * halfHSide)};
  topFace = {
      camera.m_Position,
      glm::cross(camera.getRight(), frontMultFar - camera.m_Up * halfVSide)};
  bottomFace = {
      camera.m_Position,
      glm::cross(frontMultFar + camera.m_Up * halfVSide, camera.getRight())};
}

bool Frustum::isChunkInside(const ChunkPosition& position) {
  glm::vec3 chunkCorner1 = {
      static_cast<float>(position.xPosition * Constants::Chunk::LENGTH), 0.0,
      static_cast<float>(position.zPosition * Constants::Chunk::LENGTH)};
  glm::vec3 chunkCorner2 = {chunkCorner1.x + Constants::Chunk::LENGTH,
                            Constants::Chunk::HEIGHT,
                            chunkCorner1.z + Constants::Chunk::LENGTH};

  glm::vec3 corners[8];
  int i = 0;
  for (int xi = 0; xi < 2; ++xi) {
    for (int yi = 0; yi < 2; ++yi) {
      for (int zi = 0; zi < 2; ++zi) {
        corners[i++] = glm::vec3(xi ? chunkCorner2.x : chunkCorner1.x,
                                 yi ? chunkCorner2.y : chunkCorner1.y,
                                 zi ? chunkCorner2.z : chunkCorner1.z);
      }
    }
  }

  const Plane planes[6] = {topFace,  bottomFace, rightFace,
                           leftFace, farFace,    nearFace};

  for (const Plane& p : planes) {
    int outsideCount = 0;
    for (const glm::vec3& c : corners) {
      float distance = glm::dot(c - p.point, p.normal);
      if (distance < 0.0f) {
        ++outsideCount;
      }
    }
    if (outsideCount == 8) {
      return false;
    }
  }

  return true;
}
