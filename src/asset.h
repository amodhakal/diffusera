#pragma once
#include <glm/glm.hpp>
#include <vector>

std::vector<float> getVertices(const glm::vec3& positions,
                               const glm::vec3& color) {
  float diameter = 0.7;
  float denominator = 1.0 / diameter;
  glm::vec3 newPosition = positions / denominator;

  // Normals for each face
  glm::vec3 frontNormal(0.0f, 0.0f, 1.0f);
  glm::vec3 backNormal(0.0f, 0.0f, -1.0f);
  glm::vec3 leftNormal(-1.0f, 0.0f, 0.0f);
  glm::vec3 rightNormal(1.0f, 0.0f, 0.0f);
  glm::vec3 topNormal(0.0f, 1.0f, 0.0f);
  glm::vec3 bottomNormal(0.0f, -1.0f, 0.0f);

  float x = newPosition.x;
  float y = newPosition.y;
  float z = newPosition.z;

  std::vector<float> vertices = {
      // Front face
      x + diameter / 2.0f, y + diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, frontNormal.x, frontNormal.y,
      frontNormal.z,  // Top Right
      x + diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, frontNormal.x, frontNormal.y,
      frontNormal.z,  // Bottom Right
      x - diameter / 2.0f, y + diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, frontNormal.x, frontNormal.y,
      frontNormal.z,  // Top Left

      x + diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, frontNormal.x, frontNormal.y,
      frontNormal.z,  // Bottom Right
      x - diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, frontNormal.x, frontNormal.y,
      frontNormal.z,  // Bottom Left
      x - diameter / 2.0f, y + diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, frontNormal.x, frontNormal.y,
      frontNormal.z,  // Top Left

      // Back face
      x + diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, backNormal.x, backNormal.y, backNormal.z,  // Top Right
      x + diameter / 2.0f, y - diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, backNormal.x, backNormal.y,
      backNormal.z,  // Bottom Right
      x - diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, backNormal.x, backNormal.y, backNormal.z,  // Top Left

      x + diameter / 2.0f, y - diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, backNormal.x, backNormal.y,
      backNormal.z,  // Bottom Right
      x - diameter / 2.0f, y - diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, backNormal.x, backNormal.y,
      backNormal.z,  // Bottom Left
      x - diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, backNormal.x, backNormal.y, backNormal.z,  // Top Left

      // Left face
      x - diameter / 2.0f, y + diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, leftNormal.x, leftNormal.y, leftNormal.z,  // Top Right
      x - diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, leftNormal.x, leftNormal.y,
      leftNormal.z,  // Bottom Right
      x - diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, leftNormal.x, leftNormal.y, leftNormal.z,  // Top Left

      x - diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, leftNormal.x, leftNormal.y,
      leftNormal.z,  // Bottom Right
      x - diameter / 2.0f, y - diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, leftNormal.x, leftNormal.y,
      leftNormal.z,  // Bottom Left
      x - diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, leftNormal.x, leftNormal.y, leftNormal.z,  // Top Left

      // Right face
      x + diameter / 2.0f, y + diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, rightNormal.x, rightNormal.y,
      rightNormal.z,  // Top Right
      x + diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, rightNormal.x, rightNormal.y,
      rightNormal.z,  // Bottom Right
      x + diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, rightNormal.x, rightNormal.y,
      rightNormal.z,  // Top Left

      x + diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, rightNormal.x, rightNormal.y,
      rightNormal.z,  // Bottom Right
      x + diameter / 2.0f, y - diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, rightNormal.x, rightNormal.y,
      rightNormal.z,  // Bottom Left
      x + diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, rightNormal.x, rightNormal.y,
      rightNormal.z,  // Top Left

      // Top face
      x + diameter / 2.0f, y + diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, topNormal.x, topNormal.y, topNormal.z,  // Top Right
      x - diameter / 2.0f, y + diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, topNormal.x, topNormal.y, topNormal.z,  // Bottom Right
      x + diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, topNormal.x, topNormal.y, topNormal.z,  // Top Left

      x - diameter / 2.0f, y + diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, topNormal.x, topNormal.y, topNormal.z,  // Bottom Right
      x - diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, topNormal.x, topNormal.y, topNormal.z,  // Bottom Left
      x + diameter / 2.0f, y + diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, topNormal.x, topNormal.y, topNormal.z,  // Top Left

      // Bottom face
      x + diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, bottomNormal.x, bottomNormal.y,
      bottomNormal.z,  // Top Right
      x - diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, bottomNormal.x, bottomNormal.y,
      bottomNormal.z,  // Bottom Right
      x + diameter / 2.0f, y - diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, bottomNormal.x, bottomNormal.y,
      bottomNormal.z,  // Top Left

      x - diameter / 2.0f, y - diameter / 2.0f, z + diameter / 2.0f, color.r,
      color.g, color.b, bottomNormal.x, bottomNormal.y,
      bottomNormal.z,  // Bottom Right
      x - diameter / 2.0f, y - diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, bottomNormal.x, bottomNormal.y,
      bottomNormal.z,  // Bottom Left
      x + diameter / 2.0f, y - diameter / 2.0f, z - diameter / 2.0f, color.r,
      color.g, color.b, bottomNormal.x, bottomNormal.y,
      bottomNormal.z  // Top Left
  };

  return vertices;
}
