#pragma once
#include <glm/glm.hpp>
#include <vector>

std::vector<float> getVertices(glm::vec3 positions, glm::vec3 color) {
  float halfSize = 0.5f;  // Half the size of the rectangle

  // Normals for each face
  glm::vec3 frontNormal(0.0f, 0.0f, 1.0f);
  glm::vec3 backNormal(0.0f, 0.0f, -1.0f);
  glm::vec3 leftNormal(-1.0f, 0.0f, 0.0f);
  glm::vec3 rightNormal(1.0f, 0.0f, 0.0f);
  glm::vec3 topNormal(0.0f, 1.0f, 0.0f);
  glm::vec3 bottomNormal(0.0f, -1.0f, 0.0f);

  float x = positions.x;
  float y = positions.y;
  float z = positions.z;

  std::vector<float> vertices = {
      // Front face
      x + halfSize, y + halfSize, z + halfSize, color.r, color.g, color.b,
      frontNormal.x, frontNormal.y, frontNormal.z,  // Top Right
      x + halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      frontNormal.x, frontNormal.y, frontNormal.z,  // Bottom Right
      x - halfSize, y + halfSize, z + halfSize, color.r, color.g, color.b,
      frontNormal.x, frontNormal.y, frontNormal.z,  // Top Left

      x + halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      frontNormal.x, frontNormal.y, frontNormal.z,  // Bottom Right
      x - halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      frontNormal.x, frontNormal.y, frontNormal.z,  // Bottom Left
      x - halfSize, y + halfSize, z + halfSize, color.r, color.g, color.b,
      frontNormal.x, frontNormal.y, frontNormal.z,  // Top Left

      // Back face
      x + halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      backNormal.x, backNormal.y, backNormal.z,  // Top Right
      x + halfSize, y - halfSize, z - halfSize, color.r, color.g, color.b,
      backNormal.x, backNormal.y, backNormal.z,  // Bottom Right
      x - halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      backNormal.x, backNormal.y, backNormal.z,  // Top Left

      x + halfSize, y - halfSize, z - halfSize, color.r, color.g, color.b,
      backNormal.x, backNormal.y, backNormal.z,  // Bottom Right
      x - halfSize, y - halfSize, z - halfSize, color.r, color.g, color.b,
      backNormal.x, backNormal.y, backNormal.z,  // Bottom Left
      x - halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      backNormal.x, backNormal.y, backNormal.z,  // Top Left

      // Left face
      x - halfSize, y + halfSize, z + halfSize, color.r, color.g, color.b,
      leftNormal.x, leftNormal.y, leftNormal.z,  // Top Right
      x - halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      leftNormal.x, leftNormal.y, leftNormal.z,  // Bottom Right
      x - halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      leftNormal.x, leftNormal.y, leftNormal.z,  // Top Left

      x - halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      leftNormal.x, leftNormal.y, leftNormal.z,  // Bottom Right
      x - halfSize, y - halfSize, z - halfSize, color.r, color.g, color.b,
      leftNormal.x, leftNormal.y, leftNormal.z,  // Bottom Left
      x - halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      leftNormal.x, leftNormal.y, leftNormal.z,  // Top Left

      // Right face
      x + halfSize, y + halfSize, z + halfSize, color.r, color.g, color.b,
      rightNormal.x, rightNormal.y, rightNormal.z,  // Top Right
      x + halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      rightNormal.x, rightNormal.y, rightNormal.z,  // Bottom Right
      x + halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      rightNormal.x, rightNormal.y, rightNormal.z,  // Top Left

      x + halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      rightNormal.x, rightNormal.y, rightNormal.z,  // Bottom Right
      x + halfSize, y - halfSize, z - halfSize, color.r, color.g, color.b,
      rightNormal.x, rightNormal.y, rightNormal.z,  // Bottom Left
      x + halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      rightNormal.x, rightNormal.y, rightNormal.z,  // Top Left

      // Top face
      x + halfSize, y + halfSize, z + halfSize, color.r, color.g, color.b,
      topNormal.x, topNormal.y, topNormal.z,  // Top Right
      x - halfSize, y + halfSize, z + halfSize, color.r, color.g, color.b,
      topNormal.x, topNormal.y, topNormal.z,  // Bottom Right
      x + halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      topNormal.x, topNormal.y, topNormal.z,  // Top Left

      x - halfSize, y + halfSize, z + halfSize, color.r, color.g, color.b,
      topNormal.x, topNormal.y, topNormal.z,  // Bottom Right
      x - halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      topNormal.x, topNormal.y, topNormal.z,  // Bottom Left
      x + halfSize, y + halfSize, z - halfSize, color.r, color.g, color.b,
      topNormal.x, topNormal.y, topNormal.z,  // Top Left

      // Bottom face
      x + halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      bottomNormal.x, bottomNormal.y, bottomNormal.z,  // Top Right
      x - halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      bottomNormal.x, bottomNormal.y, bottomNormal.z,  // Bottom Right
      x + halfSize, y - halfSize, z - halfSize, color.r, color.g, color.b,
      bottomNormal.x, bottomNormal.y, bottomNormal.z,  // Top Left

      x - halfSize, y - halfSize, z + halfSize, color.r, color.g, color.b,
      bottomNormal.x, bottomNormal.y, bottomNormal.z,  // Bottom Right
      x - halfSize, y - halfSize, z - halfSize, color.r, color.g, color.b,
      bottomNormal.x, bottomNormal.y, bottomNormal.z,  // Bottom Left
      x + halfSize, y - halfSize, z - halfSize, color.r, color.g, color.b,
      bottomNormal.x, bottomNormal.y, bottomNormal.z  // Top Left
  };

  return vertices;
}
