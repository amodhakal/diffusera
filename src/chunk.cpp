#include "chunk.h"

#include <glad/glad.h>
#include <noise/noise.h>

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <string>

#include "config.h"

std::vector<float> Chunk::generateMeshData(
    const glm::vec2 &position, const FastNoiseLite &noiseGenerator) {
  BlockStore blocks;
  for (uint blockX = 0; blockX < Constants::Chunk::LENGTH; blockX++) {
    for (uint blockZ = 0; blockZ < Constants::Chunk::LENGTH; blockZ++) {
      if (position.s < 0 || position.t < 0) {
        bool isHere = false;
      }

      float baseX = position.s * Constants::Chunk::LENGTH;
      float baseZ = position.t * Constants::Chunk::LENGTH;
      float noiseX = baseX + blockX;
      float noiseZ = baseZ + blockZ;

      // (-1, 1) -> (0, 1)
      float noiseY = noiseGenerator.GetNoise(noiseX, noiseZ);
      noiseY /= 2.0;
      noiseY += 0.5;

      assert(!isnan(noiseY));
      assert(noiseY >= 0.0);

      uint grassHeight = static_cast<uint>(
          std::floor(noiseY * Constants::Chunk::MAX_BLOCK_HEIGHT));

      for (int blockY = 0; blockY < grassHeight; blockY++) {
        blocks[blockX][blockY][blockZ] = BlockType::DIRT;
      }

      blocks[blockX][grassHeight][blockZ] = BlockType::GRASS;

      for (int blockY = grassHeight + 1; blockY < Constants::Chunk::HEIGHT;
           blockY++) {
        blocks[blockX][blockY][blockZ] = BlockType::AIR;
      }
    }
  }

  std::vector<float> vertices;

  int chunkXToPosition = 0;
  int chunkZToPosition = 0;

  for (uint blockX = 0; blockX < Constants::Chunk::LENGTH; blockX++) {
    for (uint blockY = 0; blockY < Constants::Chunk::HEIGHT; blockY++) {
      for (uint blockZ = 0; blockZ < Constants::Chunk::LENGTH; blockZ++) {
        BlockType current = blocks[blockX][blockY][blockZ];
        glm::vec3 color;

        switch (current) {
          case BlockType::AIR:
            continue;
          case BlockType::GRASS:
            color = {0.0, 0.6, 0.1};
            break;
          case BlockType::DIRT:
            color = {0.6, 0.2, 0.2};
            break;
          default:
            throw std::runtime_error("Invalid type received: " +
                                     std::to_string(current));
        }

        // Air has no mesh
        if (current == BlockType::AIR) {
          continue;
        }

        std::vector<float> currentData;
        float x, y, z;

        // Top
        if (blockY + 1 == Constants::Chunk::HEIGHT ||
            blocks[blockX][blockY + 1][blockZ] == BlockType::AIR) {
          x = blockX;
          y = blockY + 1;
          z = blockZ;
          currentData = {
              // First triangle
              x, y, z, BlockNormal::TOP_NORMAL, color[0], color[1],
              color[2],  // bottom-left
              x + 1, y, z, BlockNormal::TOP_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x, y, z + 1, BlockNormal::TOP_NORMAL, color[0], color[1],
              color[2],  // top-left

              // Second triangle
              x + 1, y, z, BlockNormal::TOP_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x + 1, y, z + 1, BlockNormal::TOP_NORMAL, color[0], color[1],
              color[2],  // top-right
              x, y, z + 1, BlockNormal::TOP_NORMAL, color[0], color[1],
              color[2]  // top-left
          };

          vertices.insert_range(vertices.end(), currentData);
        }

        // Down
        if (blockY == 0 ||
            blocks[blockX][blockY - 1][blockZ] == BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY;
          z = chunkZToPosition + blockZ;

          currentData = {
              // First triangle
              x, y, z, BlockNormal::BOTTOM_NORMAL, color[0], color[1],
              color[2],  // bottom-left
              x + 1, y, z, BlockNormal::BOTTOM_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x, y, z + 1, BlockNormal::BOTTOM_NORMAL, color[0], color[1],
              color[2],  // top-left

              // Second triangle
              x + 1, y, z, BlockNormal::BOTTOM_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x + 1, y, z + 1, BlockNormal::BOTTOM_NORMAL, color[0], color[1],
              color[2],  // top-right
              x, y, z + 1, BlockNormal::BOTTOM_NORMAL, color[0], color[1],
              color[2]  // top-left
          };

          vertices.insert_range(vertices.end(), currentData);
        }

        // Front
        if (blockZ + 1 == Constants::Chunk::LENGTH ||
            blocks[blockX][blockY][blockZ + 1] == BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY;
          z = chunkZToPosition + blockZ + 1;

          currentData = {
              // First triangle
              x, y, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2],  // bottom-left
              x + 1, y, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x, y + 1, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2],  // top-left

              // Second triangle
              x + 1, y, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x + 1, y + 1, z, BlockNormal::FRONT_BACK_NORMAL, color[0],
              color[1],
              color[2],  // top-right
              x, y + 1, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2]  // top-left
          };

          vertices.insert_range(vertices.end(), currentData);
        }

        // Back
        if (blockZ == 0 ||
            blocks[blockX][blockY][blockZ - 1] == BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY;
          z = chunkZToPosition + blockZ;

          currentData = {
              // First triangle
              x, y, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2],  // bottom-left
              x + 1, y, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x, y + 1, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2],  // top-left

              // Second triangle
              x + 1, y, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x + 1, y + 1, z, BlockNormal::FRONT_BACK_NORMAL, color[0],
              color[1],
              color[2],  // top-right
              x, y + 1, z, BlockNormal::FRONT_BACK_NORMAL, color[0], color[1],
              color[2]  // top-left
          };

          vertices.insert_range(vertices.end(), currentData);
        }

        // Left
        if (blockX == 0 ||
            blocks[blockX - 1][blockY][blockZ] == BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY;
          z = chunkZToPosition + blockZ;

          currentData = {
              // First triangle
              x, y, z, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2],  // bottom-left
              x, y, z + 1, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x, y + 1, z, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2],  // top-left

              // Second triangle
              x, y, z + 1, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x, y + 1, z + 1, BlockNormal::RIGHT_LEFT_NORMAL, color[0],
              color[1],
              color[2],  // top-right
              x, y + 1, z, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2]  // top-left
          };

          vertices.insert_range(vertices.end(), currentData);
        }

        // Right
        if (blockX + 1 == Constants::Chunk::LENGTH ||
            blocks[blockX + 1][blockY][blockZ] == BlockType::AIR) {
          x = chunkXToPosition + blockX + 1;
          y = blockY;
          z = chunkZToPosition + blockZ;

          currentData = {
              // First triangle
              x, y, z, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2],  // bottom-left
              x, y, z + 1, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x, y + 1, z, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2],  // top-left

              // Second triangle
              x, y, z + 1, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2],  // bottom-right
              x, y + 1, z + 1, BlockNormal::RIGHT_LEFT_NORMAL, color[0],
              color[1],
              color[2],  // top-right
              x, y + 1, z, BlockNormal::RIGHT_LEFT_NORMAL, color[0], color[1],
              color[2]  // top-left
          };

          vertices.insert_range(vertices.end(), currentData);
        }
      }
    }
  }

  return vertices;
}

void Chunk::useMeshData(std::vector<float> &meshData) {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(float),
               meshData.data(), GL_STATIC_DRAW);

  // Clean up the large vbo data once setup the chunk
  m_VboSize = meshData.size();
  meshData.clear();
  meshData.shrink_to_fit();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                        (void *)(4 * sizeof(float)));
  glEnableVertexAttribArray(2);

  if (Constants::DO_TRIANGLE_LINE) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
}

void Chunk::cleanup() {
  glDeleteBuffers(1, &m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
}

void Chunk::render() {
  glBindVertexArray(m_VAO);
  // Each vertex has 7 floats (position.xyz + normal + color.rgb).
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_VboSize / 7));
}
