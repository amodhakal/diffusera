#include <__ostream/print.h>
#define GL_SILENCE_DEPRECATION

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>

#include "chunk.h"
#include "config.h"
#include "frustum.h"
#include "manager.h"

ChunkManager::ChunkManager() {
  float seed = std::rand();

  m_NoiseGenerator = FastNoiseLite(seed);
  m_NoiseGenerator.SetNoiseType(Constants::Noise::NOISE_TYPE);
  m_NoiseGenerator.SetFractalType(Constants::Noise::FRACTAL_TYPE);
  m_NoiseGenerator.SetFractalOctaves(Constants::Noise::FRACTAL_OCTAVE);
  m_NoiseGenerator.SetFractalGain(Constants::Noise::FRACTAL_GAIN);
  m_NoiseGenerator.SetFractalLacunarity(Constants::Noise::FRACTAL_LACUNARITY);
  m_NoiseGenerator.SetFrequency(Constants::Noise::FREQUENCY);
}

void ChunkManager::load(const Shader& shader) {
  m_Shader = shader;
}

void ChunkManager::render(const Camera& camera) {
  glm::vec3 cameraPosition = camera.m_Position;

  for (auto it = m_Chunks.begin(); it != m_Chunks.end();) {
    const ChunkPosition& position = it->first;
    float chunkCenterX = position.xPosition * Constants::Chunk::LENGTH -
                         (Constants::Chunk::LENGTH / 2.0f);
    float chunkCenterZ = position.zPosition * Constants::Chunk::LENGTH -
                         (Constants::Chunk::LENGTH / 2.0f);

    float xDisplacement = chunkCenterX - cameraPosition.x;
    float zDisplacement = chunkCenterZ - cameraPosition.z;

    float distanceSquared =
        (xDisplacement * xDisplacement) + (zDisplacement * zDisplacement);
    if (distanceSquared > Constants::Chunk::RENDER_DISTANCE_BLOCKS *
                              Constants::Chunk::RENDER_DISTANCE_BLOCKS) {
      it = m_Chunks.erase(it);
      continue;
    }

    ++it;
  }

  int currentChunkX = static_cast<int>(
      std::floor((cameraPosition.x + (Constants::Chunk::LENGTH / 2.0f)) /
                 Constants::Chunk::LENGTH));
  int currentChunkZ = static_cast<int>(
      std::floor((cameraPosition.z + (Constants::Chunk::LENGTH / 2.0f)) /
                 Constants::Chunk::LENGTH));

  for (int chunkX = currentChunkX - Constants::Chunk::RENDER_DISTANCE_CHUNKS;
       chunkX <= currentChunkX + Constants::Chunk::RENDER_DISTANCE_CHUNKS;
       chunkX++) {
    for (int chunkZ = currentChunkZ - Constants::Chunk::RENDER_DISTANCE_CHUNKS;
         chunkZ <= currentChunkZ + Constants::Chunk::RENDER_DISTANCE_CHUNKS;
         chunkZ++) {
      ChunkPosition position = {chunkX, chunkZ};
      float chunkCenterX = position.xPosition * Constants::Chunk::LENGTH -
                           (Constants::Chunk::LENGTH / 2.0f);
      float chunkCenterZ = position.zPosition * Constants::Chunk::LENGTH -
                           (Constants::Chunk::LENGTH / 2.0f);

      float xDisplacement = chunkCenterX - cameraPosition.x;
      float zDisplacement = chunkCenterZ - cameraPosition.z;

      float distanceSquared =
          (xDisplacement * xDisplacement) + (zDisplacement * zDisplacement);
      if (distanceSquared > Constants::Chunk::RENDER_DISTANCE_BLOCKS *
                                Constants::Chunk::RENDER_DISTANCE_BLOCKS) {
        // Chunk outside of the radius
        continue;
      }

      if (m_Chunks.find(position) == m_Chunks.end()) {
        m_Chunks.emplace(position, Chunk(position, m_NoiseGenerator));
      }
    }
  }

  glUseProgram(m_Shader.getId());
  Frustum frustum(camera, camera.m_Aspect, camera.m_Fov,
                  Constants::Camera::NEAR, Constants::Camera::FAR);

  for (auto& value : m_Chunks) {
    const ChunkPosition& position = value.first;
    if (!frustum.isChunkInside(position)) {
      continue;
    }

    Chunk& chunk = value.second;
    chunk.render();
  }
}
