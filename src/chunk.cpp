#include "chunk.h"
#include <noise/noise.h>

#include <cassert>
#include <cmath>

#include "config.h"

void Chunk::generateMeshData(const glm::vec2 &position) {
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

      auto n = Noise::fbm(
          glm::vec2(noiseX, noiseZ) * Constants::Noise::FREQUENCY,
          Constants::Noise::FRACTAL_OCTAVE,
          Constants::Noise::FRACTAL_LACUNARITY, Constants::Noise::FRACTAL_GAIN);
      float noiseY = n.value;
      noiseY /= 2.0f;
      noiseY += 0.5f;

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

  const int dims[3] = {static_cast<int>(BX), static_cast<int>(BY),
                       static_cast<int>(BZ)};

  auto idx3 = [&](int x, int y, int z) -> size_t {
    return (static_cast<size_t>(x) * BY + static_cast<size_t>(y)) * BZ +
           static_cast<size_t>(z);
  };

  // Map (block type, face normal) -> texture unit id
  auto blockTextureId = [&](BlockType t, BlockNormal n) -> int {
    switch (t) {
    case BlockType::GRASS:
      if (n == BlockNormal::TOP_NORMAL)
        return 0; // grass_top
      if (n == BlockNormal::BOTTOM_NORMAL)
        return 2; // dirt underside
      return 1;   // grass_side
    case BlockType::DIRT:
      return 2; // dirt
    default:
      return 2;
    }
  };

  auto addQuad = [&](const glm::ivec3 &a, const glm::ivec3 &du,
                     const glm::ivec3 &dv, BlockNormal normalId, int texId,
                     bool flipV) {
    glm::vec3 p0 = glm::vec3(static_cast<float>(a.x), static_cast<float>(a.y),
                             static_cast<float>(a.z));
    glm::vec3 p1 = glm::vec3(static_cast<float>(a.x + du.x),
                             static_cast<float>(a.y + du.y),
                             static_cast<float>(a.z + du.z));
    glm::vec3 p2 = glm::vec3(static_cast<float>(a.x + dv.x),
                             static_cast<float>(a.y + dv.y),
                             static_cast<float>(a.z + dv.z));
    glm::vec3 p3 = glm::vec3(static_cast<float>(a.x + du.x + dv.x),
                             static_cast<float>(a.y + du.y + dv.y),
                             static_cast<float>(a.z + du.z + dv.z));

    size_t base = m_Data.size() / 7;

    auto pushVertex = [&](const glm::vec3 &p, const glm::vec2 &uv) {
      m_Data.push_back(p.x);
      m_Data.push_back(p.y);
      m_Data.push_back(p.z);
      m_Data.push_back(static_cast<float>(normalId));
      m_Data.push_back(static_cast<float>(texId));
      m_Data.push_back(uv.x);
      m_Data.push_back(uv.y);
    };

    float width =
        static_cast<float>(std::abs(du.x) + std::abs(du.y) + std::abs(du.z));
    float height =
        static_cast<float>(std::abs(dv.x) + std::abs(dv.y) + std::abs(dv.z));

    if (!flipV) {
      pushVertex(p0, glm::vec2(0.0f, 0.0f));
      pushVertex(p1, glm::vec2(width, 0.0f));
      pushVertex(p2, glm::vec2(0.0f, height));
      pushVertex(p3, glm::vec2(width, height));
    } else {
      pushVertex(p0, glm::vec2(0.0f, height));
      pushVertex(p1, glm::vec2(width, height));
      pushVertex(p2, glm::vec2(0.0f, 0.0f));
      pushVertex(p3, glm::vec2(width, 0.0f));
    }

    m_Indices.push_back(static_cast<uint>(base + 0));
    m_Indices.push_back(static_cast<uint>(base + 2));
    m_Indices.push_back(static_cast<uint>(base + 1));
    m_Indices.push_back(static_cast<uint>(base + 1));
    m_Indices.push_back(static_cast<uint>(base + 2));
    m_Indices.push_back(static_cast<uint>(base + 3));
  };

  for (int d = 0; d < 3; ++d) {
    int u = (d + 1) % 3;
    int v = (d + 2) % 3;

    for (int dir = -1; dir <= 1; dir += 2) {
      int dimU = dims[u];
      int dimV = dims[v];
      int dimD = dims[d];

      std::vector<int> mask(dimU * dimV, 0);

      for (int slice = 0; slice < dimD; ++slice) {
        for (int iv = 0; iv < dimV; ++iv) {
          for (int iu = 0; iu < dimU; ++iu) {
            int coord[3];
            coord[d] = slice;
            coord[u] = iu;
            coord[v] = iv;

            int nx = coord[0];
            int ny = coord[1];
            int nz = coord[2];

            int nn[3] = {nx, ny, nz};
            nn[d] = coord[d] + dir;

            BlockType a = BlockType::AIR;
            BlockType b = BlockType::AIR;

            if (coord[0] >= 0 && coord[0] < dims[0] && coord[1] >= 0 &&
                coord[1] < dims[1] && coord[2] >= 0 && coord[2] < dims[2]) {
              a = blocks[idx3(coord[0], coord[1], coord[2])];
            }

            if (nn[0] >= 0 && nn[0] < dims[0] && nn[1] >= 0 &&
                nn[1] < dims[1] && nn[2] >= 0 && nn[2] < dims[2]) {
              b = blocks[idx3(nn[0], nn[1], nn[2])];
            }

            int maskIndex = iv * dimU + iu;
            if (a != BlockType::AIR && b == BlockType::AIR) {
              mask[maskIndex] = static_cast<int>(a) + 1;
            } else {
              mask[maskIndex] = 0;
            }
          }
        }

        for (int j = 0; j < dimV; ++j) {
          for (int i = 0; i < dimU; ++i) {
            int mIndex = j * dimU + i;
            int id = mask[mIndex];
            if (id == 0)
              continue;

            int width = 1;
            while (i + width < dimU && mask[j * dimU + (i + width)] == id)
              ++width;

            int height = 1;
            bool done = false;
            while (!done && j + height < dimV) {
              for (int k2 = 0; k2 < width; ++k2) {
                if (mask[(j + height) * dimU + (i + k2)] != id) {
                  done = true;
                  break;
                }
              }
              if (!done)
                ++height;
            }

            int aCoord[3] = {0, 0, 0};
            aCoord[d] = slice + (dir == 1 ? 1 : 0);
            aCoord[u] = i;
            aCoord[v] = j;

            glm::ivec3 a3 = glm::ivec3(aCoord[0], aCoord[1], aCoord[2]);

            int duArr[3] = {0, 0, 0};
            int dvArr[3] = {0, 0, 0};
            duArr[u] = width;
            dvArr[v] = height;

            glm::ivec3 du3 = glm::ivec3(duArr[0], duArr[1], duArr[2]);
            glm::ivec3 dv3 = glm::ivec3(dvArr[0], dvArr[1], dvArr[2]);

            BlockNormal normalId;
            if (d == 0) {
              normalId = BlockNormal::RIGHT_LEFT_NORMAL;
            } else if (d == 1) {
              normalId = (dir == 1) ? BlockNormal::TOP_NORMAL
                                    : BlockNormal::BOTTOM_NORMAL;
            } else {
              normalId = BlockNormal::FRONT_BACK_NORMAL;
            }

            BlockType mat = static_cast<BlockType>(id - 1);
            int texId = blockTextureId(mat, normalId);

            bool flipV = (d == 2 && dir == -1);
            addQuad(a3, du3, dv3, normalId, texId, flipV);

            for (int hj = 0; hj < height; ++hj) {
              for (int wi = 0; wi < width; ++wi) {
                mask[(j + hj) * dimU + (i + wi)] = 0;
              }
            }

            i += width - 1;
          }
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
