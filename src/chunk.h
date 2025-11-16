#pragma once

#include <vector>

#include "asset.h"
#include "config.h"

enum class Block {
  AIR,
  GRASS,
};

constexpr glm::vec3 blockColors[] = {{0, 0, 0}, {0.2, 0.8, 0.3}};

class Chunk {
 public:
  Chunk(int xPosition, int yPosition);
  std::vector<float> getVboData();

 private:
  Block m_Blocks[Constants::Chunk::Y_SIZE][Constants::Chunk::Z_SIZE]
                [Constants::Chunk::X_SIZE];
  void setBlock(uint length, uint width, uint height, Block block);
  Block getBlock(uint length, uint width, uint height);
};

Chunk::Chunk(int xPosition, int yPosition) {
  for (uint height = 0; height < Constants::Chunk::Y_SIZE; height++) {
    for (uint width = 0; width < Constants::Chunk::Z_SIZE; width++) {
      for (uint length = 0; length < Constants::Chunk::X_SIZE; length++) {
        setBlock(length, width, height, Block::GRASS);
      }
    }
  }
}

std::vector<float> Chunk::getVboData() {
  std::vector<float> data;

  for (uint height = 0; height < Constants::Chunk::Y_SIZE; height++) {
    for (uint width = 0; width < Constants::Chunk::Z_SIZE; width++) {
      for (uint length = 0; length < Constants::Chunk::X_SIZE; length++) {
        Block block = getBlock(length, width, height);
        if (block == Block::AIR) {
          continue;
        }

        glm::vec3 color = blockColors[(uint)block];
        std::vector blockData = getVertices({length, height, width}, color);
        data.insert_range(data.end(), blockData);
      }
    }
  }

  return data;
}

void Chunk::setBlock(uint length, uint width, uint height, Block block) {
  m_Blocks[height][width][length] = block;
}

Block Chunk::getBlock(uint length, uint width, uint height) {
  return m_Blocks[height][width][length];
}
