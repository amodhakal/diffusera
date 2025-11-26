#include "manager.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>

#include "chunk.h"
#include "config.h"

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

void ChunkManager::load() {
}

void ChunkManager::render(const Camera& camera, Shader& shader) {
  glm::vec3 cameraPosition = camera.m_Position;

  for (auto it = m_ProcessedChunks.begin(); it != m_ProcessedChunks.end();) {
    const glm::vec2& position = it->first;
    float chunkCenterX = position.s * Constants::Chunk::LENGTH -
                         (Constants::Chunk::LENGTH / 2.0f);
    float chunkCenterZ = position.t * Constants::Chunk::LENGTH -
                         (Constants::Chunk::LENGTH / 2.0f);

    float xDisplacement = chunkCenterX - cameraPosition.x;
    float zDisplacement = chunkCenterZ - cameraPosition.z;

    float distanceSquared =
        (xDisplacement * xDisplacement) + (zDisplacement * zDisplacement);
    if (distanceSquared > Constants::Chunk::RENDER_DISTANCE_BLOCKS *
                              Constants::Chunk::RENDER_DISTANCE_BLOCKS) {
      it->second.cleanup();
      it = m_ProcessedChunks.erase(it);
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
      glm::vec2 position = {chunkX, chunkZ};
      float chunkCenterX = position.s * Constants::Chunk::LENGTH -
                           (Constants::Chunk::LENGTH / 2.0f);
      float chunkCenterZ = position.t * Constants::Chunk::LENGTH -
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

      if (m_ProcessedChunks.contains(position)) {
        // Chunk already processed
        continue;
      }

      if (m_ProcessingChunks.contains(position)) {
        // Chunk in the process of being processed;
        continue;
      }

      m_ProcessingChunks.insert(position);

      Chunk chunk;
      std::vector<float> meshData = chunk.generateMeshData(
          position, m_NoiseGenerator);  // The generation can be parallized
                                        // since no gl calls
      chunk.useMeshData(
          meshData);  // require gl calls - requires the main thread

      m_ProcessingChunks.erase(position);
      m_ProcessedChunks[position] = chunk;
    }
  }

  shader.use();

  for (auto& value : m_ProcessedChunks) {
    const glm::vec2& position = value.first;

    if (!camera.isChunkInside(position)) {
      // TODO Fix frustum
      continue;
    }

    Chunk& chunk = value.second;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(
        model,
        glm::vec3(static_cast<float>(position.s * Constants::Chunk::LENGTH),
                  0.0f,
                  static_cast<float>(position.t * Constants::Chunk::LENGTH)));
    shader.setUniformMat4("uModel", model);

    chunk.render();
  }
}
