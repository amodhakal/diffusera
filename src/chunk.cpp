#include "chunk.h"

#include <__ostream/print.h>
#include <glad/glad.h>
#include <noise/noise.h>

#include <cassert>
#include <cmath>

#include "config.h"

Chunk::Chunk(const ChunkPosition &position,
             const FastNoiseLite &noiseGenerator) {
  BlockStore blocks;
  for (uint blockX = 0; blockX < Constants::Chunk::LENGTH; blockX++) {
    for (uint blockZ = 0; blockZ < Constants::Chunk::LENGTH; blockZ++) {
      float noiseX = position.xPosition * Constants::Chunk::LENGTH;
      noiseX += blockX;
      float noiseZ = position.zPosition * Constants::Chunk::LENGTH;
      noiseZ += blockZ;

      // (-1, 1) -> (0, 1)
      float noiseY = noiseGenerator.GetNoise(noiseX, noiseZ);
      noiseY /= 2.0;
      noiseY += 0.5;

      assert(!isnan(noiseY));
      assert(noiseY >= 0.0);

      uint grassHeight = static_cast<uint>(
          std::floor(noiseY * Constants::Chunk::MAX_BLOCK_HEIGHT));

      for (int blockY = 0; blockY < grassHeight; blockY++) {
        blocks[blockX][blockY][blockZ] = BlockType::GRASS;
      }

      for (int blockY = grassHeight; blockY < Constants::Chunk::HEIGHT;
           blockY++) {
        blocks[blockX][blockY][blockZ] = BlockType::AIR;
      }
    }
  }

  float baseX = position.xPosition * Constants::Chunk::LENGTH;
  float baseZ = position.zPosition * Constants::Chunk::LENGTH;
  m_VboData = getVboFromStore(blocks, position);

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_VboData.size() * sizeof(float),
               m_VboData.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  if (Constants::DO_TRIANGLE_LINE) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
}

void Chunk::render() {
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_VboData.size() / 9);
}

std::vector<float> Chunk::getVboFromStore(const BlockStore &blocks,
                                          const ChunkPosition &position) {
  std::vector<float> vertices;

  int chunkXToPosition = position.xPosition * Constants::Chunk::LENGTH;
  int chunkZToPosition = position.zPosition * Constants::Chunk::LENGTH;

  for (uint blockX = 0; blockX < Constants::Chunk::LENGTH; blockX++) {
    for (uint blockY = 0; blockY < Constants::Chunk::HEIGHT; blockY++) {
      for (uint blockZ = 0; blockZ < Constants::Chunk::LENGTH; blockZ++) {
        BlockType current = blocks[blockX][blockY][blockZ];

        // Air has no mesh
        if (current == BlockType::AIR) {
          continue;
        }

        std::vector<float> currentData;
        float x, y, z;

        // Top
        if (blockY + 1 == Constants::Chunk::HEIGHT ||
            blocks[blockX][blockY + 1][blockZ] == BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY + 1;
          z = chunkZToPosition + blockZ;

          currentData = {
              // First triangle
              x, y, z, 0.0, 1.0, 0.0, 0.0, 0.6, 0.1,      // bottom-left
              x + 1, y, z, 0.0, 1.0, 0.0, 0.0, 0.6, 0.1,  // bottom-right
              x, y, z + 1, 0.0, 1.0, 0.0, 0.0, 0.6, 0.1,  // top-left

              // Second triangle
              x + 1, y, z, 0.0, 1.0, 0.0, 0.0, 0.6, 0.1,      // bottom-right
              x + 1, y, z + 1, 0.0, 1.0, 0.0, 0.0, 0.6, 0.1,  // top-right
              x, y, z + 1, 0.0, 1.0, 0.0, 0.0, 0.6, 0.1       // top-left
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
              x, y, z, 0.0, -1.0, 0.0, 0.0, 0.6, 0.1,      // bottom-left
              x + 1, y, z, 0.0, -1.0, 0.0, 0.0, 0.6, 0.1,  // bottom-right
              x, y, z + 1, 0.0, -1.0, 0.0, 0.0, 0.6, 0.1,  // top-left

              // Second triangle
              x + 1, y, z, 0.0, -1.0, 0.0, 0.0, 0.6, 0.1,  // bottom-right
              x + 1, y, z + 1, 0.0, -1.0, 0.0, 0.0, 0.6,
              0.1,                                        // top-right
              x, y, z + 1, 0.0, -1.0, 0.0, 0.0, 0.6, 0.1  // top-left
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
              x, y, z, 0.0, 0.0, 1.0, 0.0, 0.6, 0.1,      // bottom-left
              x + 1, y, z, 0.0, 0.0, 1.0, 0.0, 0.6, 0.1,  // bottom-right
              x, y + 1, z, 0.0, 0.0, 1.0, 0.0, 0.6, 0.1,  // top-left

              // Second triangle
              x + 1, y, z, 0.0, 0.0, 1.0, 0.0, 0.6, 0.1,      // bottom-right
              x + 1, y + 1, z, 0.0, 0.0, 1.0, 0.0, 0.6, 0.1,  // top-right
              x, y + 1, z, 0.0, 0.0, 1.0, 0.0, 0.6, 0.1       // top-left
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
              x, y, z, 0.0, 0.0, -1.0, 0.0, 0.6, 0.1,      // bottom-left
              x + 1, y, z, 0.0, 0.0, -1.0, 0.0, 0.6, 0.1,  // bottom-right
              x, y + 1, z, 0.0, 0.0, -1.0, 0.0, 0.6, 0.1,  // top-left

              // Second triangle
              x + 1, y, z, 0.0, 0.0, -1.0, 0.0, 0.6, 0.1,  // bottom-right
              x + 1, y + 1, z, 0.0, 0.0, -1.0, 0.0, 0.6,
              0.1,                                        // top-right
              x, y + 1, z, 0.0, 0.0, -1.0, 0.0, 0.6, 0.1  // top-left
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
              x, y, z, -1.0, 0.0, 0.0, 0.0, 0.6, 0.1,      // bottom-left
              x, y, z + 1, -1.0, 0.0, 0.0, 0.0, 0.6, 0.1,  // bottom-right
              x, y + 1, z, -1.0, 0.0, 0.0, 0.0, 0.6, 0.1,  // top-left

              // Second triangle
              x, y, z + 1, -1.0, 0.0, 0.0, 0.0, 0.6, 0.1,  // bottom-right
              x, y + 1, z + 1, -1.0, 0.0, 0.0, 0.0, 0.6,
              0.1,                                        // top-right
              x, y + 1, z, -1.0, 0.0, 0.0, 0.0, 0.6, 0.1  // top-left
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
              x, y, z, 1.0, 0.0, 0.0, 0.0, 0.6, 0.1,      // bottom-left
              x, y, z + 1, 1.0, 0.0, 0.0, 0.0, 0.6, 0.1,  // bottom-right
              x, y + 1, z, 1.0, 0.0, 0.0, 0.0, 0.6, 0.1,  // top-left

              // Second triangle
              x, y, z + 1, 1.0, 0.0, 0.0, 0.0, 0.6, 0.1,      // bottom-right
              x, y + 1, z + 1, 1.0, 0.0, 0.0, 0.0, 0.6, 0.1,  // top-right
              x, y + 1, z, 1.0, 0.0, 0.0, 0.0, 0.6, 0.1f      // top-left
          };

          vertices.insert_range(vertices.end(), currentData);
        }
      }
    }
  }

  return vertices;
}
