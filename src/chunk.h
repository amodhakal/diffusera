#pragma once

struct ChunkPosition {
  int xPosition;
  int yPosition;

  bool operator==(const ChunkPosition& other) {
    return other.xPosition == xPosition && other.yPosition == yPosition;
  }
};

class Chunk {};
