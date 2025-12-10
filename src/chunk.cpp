#include "chunk.h"

#include <noise/noise.h>

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <string>

#include "config.h"

void Chunk::generateMeshData(const glm::vec2 &position,
                             const FastNoiseLite &noiseGenerator) {
  // Use a heap-allocated flat buffer for block storage so we don't keep a
  // large fixed-size member inside each Chunk instance after generation.
  const size_t BX = Constants::Chunk::LENGTH;
  const size_t BY = Constants::Chunk::HEIGHT;
  const size_t BZ = Constants::Chunk::LENGTH;
  std::vector<BlockType> blocks(BX * BY * BZ, BlockType::AIR);

  auto blockIndex = [&](int x, int y, int z) -> size_t {
    return (static_cast<size_t>(x) * BY + static_cast<size_t>(y)) * BZ +
           static_cast<size_t>(z);
  };

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
      m_HeightMap[blockX][blockZ] = grassHeight;

      for (int blockY = 0; blockY < grassHeight; blockY++) {
        blocks[blockIndex(blockX, blockY, blockZ)] = BlockType::DIRT;
      }

      blocks[blockIndex(blockX, grassHeight, blockZ)] = BlockType::GRASS;

      for (int blockY = grassHeight + 1; blockY < Constants::Chunk::HEIGHT;
           blockY++) {
        blocks[blockIndex(blockX, blockY, blockZ)] = BlockType::AIR;
      }
    }
  }

  int chunkXToPosition = 0;
  int chunkZToPosition = 0;

  m_Data.clear();
  m_Indices.clear();

  for (uint blockX = 0; blockX < Constants::Chunk::LENGTH; blockX++) {
    for (uint blockY = 0; blockY < Constants::Chunk::HEIGHT; blockY++) {
      for (uint blockZ = 0; blockZ < Constants::Chunk::LENGTH; blockZ++) {
        BlockType current = blocks[blockIndex(blockX, blockY, blockZ)];
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

        float x, y, z;
        // Top
        if (blockY + 1 == Constants::Chunk::HEIGHT ||
            blocks[blockIndex(blockX, blockY + 1, blockZ)] ==
              BlockType::AIR) {
          x = blockX;
          y = blockY + 1;
          z = blockZ;
          size_t idx = m_Data.size() / 7;
          m_Data.insert(m_Data.end(), {x,
                                       y,
                                       z,
                                       BlockNormal::TOP_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z,
                                       BlockNormal::TOP_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y,
                                       z + 1,
                                       BlockNormal::TOP_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z,
                                       BlockNormal::TOP_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z + 1,
                                       BlockNormal::TOP_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y,
                                       z + 1,
                                       BlockNormal::TOP_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2]});
          m_Indices.insert(m_Indices.end(),
                           {(uint)idx, (uint)idx + 1, (uint)idx + 2,
                            (uint)idx + 3, (uint)idx + 4, (uint)idx + 5});
        }
        // Bottom
        if (blockY == 0 ||
            blocks[blockIndex(blockX, blockY - 1, blockZ)] ==
              BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY;
          z = chunkZToPosition + blockZ;
          size_t idx = m_Data.size() / 7;
          m_Data.insert(m_Data.end(), {x,
                                       y,
                                       z,
                                       BlockNormal::BOTTOM_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z,
                                       BlockNormal::BOTTOM_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y,
                                       z + 1,
                                       BlockNormal::BOTTOM_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z,
                                       BlockNormal::BOTTOM_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z + 1,
                                       BlockNormal::BOTTOM_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y,
                                       z + 1,
                                       BlockNormal::BOTTOM_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2]});
          m_Indices.insert(m_Indices.end(),
                           {(uint)idx, (uint)idx + 1, (uint)idx + 2,
                            (uint)idx + 3, (uint)idx + 4, (uint)idx + 5});
        }
        // Front
        if (blockZ + 1 == Constants::Chunk::LENGTH ||
            blocks[blockIndex(blockX, blockY, blockZ + 1)] ==
              BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY;
          z = chunkZToPosition + blockZ + 1;
          size_t idx = m_Data.size() / 7;
          m_Data.insert(m_Data.end(), {x,
                                       y,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y + 1,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2]});
          m_Indices.insert(m_Indices.end(),
                           {(uint)idx, (uint)idx + 1, (uint)idx + 2,
                            (uint)idx + 3, (uint)idx + 4, (uint)idx + 5});
        }
        // Back
        if (blockZ == 0 ||
            blocks[blockIndex(blockX, blockY, blockZ - 1)] ==
              BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY;
          z = chunkZToPosition + blockZ;
          size_t idx = m_Data.size() / 7;
          m_Data.insert(m_Data.end(), {x,
                                       y,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x + 1,
                                       y + 1,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z,
                                       BlockNormal::FRONT_BACK_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2]});
          m_Indices.insert(m_Indices.end(),
                           {(uint)idx, (uint)idx + 1, (uint)idx + 2,
                            (uint)idx + 3, (uint)idx + 4, (uint)idx + 5});
        }
        // Left
        if (blockX == 0 ||
            blocks[blockIndex(blockX - 1, blockY, blockZ)] ==
              BlockType::AIR) {
          x = chunkXToPosition + blockX;
          y = blockY;
          z = chunkZToPosition + blockZ;
          size_t idx = m_Data.size() / 7;
          m_Data.insert(m_Data.end(), {x,
                                       y,
                                       z,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y,
                                       z + 1,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y,
                                       z + 1,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z + 1,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2]});
          m_Indices.insert(m_Indices.end(),
                           {(uint)idx, (uint)idx + 1, (uint)idx + 2,
                            (uint)idx + 3, (uint)idx + 4, (uint)idx + 5});
        }
        // Right
        if (blockX + 1 == Constants::Chunk::LENGTH ||
            blocks[blockIndex(blockX + 1, blockY, blockZ)] ==
              BlockType::AIR) {
          x = chunkXToPosition + blockX + 1;
          y = blockY;
          z = chunkZToPosition + blockZ;
          size_t idx = m_Data.size() / 7;
          m_Data.insert(m_Data.end(), {x,
                                       y,
                                       z,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y,
                                       z + 1,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y,
                                       z + 1,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z + 1,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2],
                                       x,
                                       y + 1,
                                       z,
                                       BlockNormal::RIGHT_LEFT_NORMAL,
                                       color[0],
                                       color[1],
                                       color[2]});
          m_Indices.insert(m_Indices.end(),
                           {(uint)idx, (uint)idx + 1, (uint)idx + 2,
                            (uint)idx + 3, (uint)idx + 4, (uint)idx + 5});
        }
      }
    }
  }
}

void Chunk::pass() {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_Data.size() * sizeof(float), m_Data.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint),
               m_Indices.data(), GL_STATIC_DRAW);

  m_IndexCount = static_cast<uint>(m_Indices.size());
  m_VboSize = m_Data.size();

  m_Data.clear();
  m_Indices.clear();
  m_Data.shrink_to_fit();
  m_Indices.shrink_to_fit();

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
  glDeleteBuffers(1, &m_EBO);
  glDeleteVertexArrays(1, &m_VAO);
}

void Chunk::render() {
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_IndexCount),
                 GL_UNSIGNED_INT, 0);
}

uint Chunk::getHighestBlockY(uint blockX, uint blockZ) {
  return m_HeightMap[blockX][blockZ];
}
