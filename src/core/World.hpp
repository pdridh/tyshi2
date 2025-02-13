#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Engine.hpp"

#include <vector>
#include <random>
#include <algorithm>

struct Tile
{
  int atlasCode;
  Tile(int code = -1) : atlasCode{code} {}
};

enum TileType
{
  Grass,
  Block,
};

enum Biome
{
  Grassland,
  Forest,
  Rocky,
  MAX_BIOME_TYPE,
};

struct Chunk
{

  std::vector<Tile *> tiles;
  Biome closestBiome;
  Chunk(int nTiles, Biome b)
      : closestBiome{b}
  {
    // Storing 2d in 1d
    tiles.resize(nTiles * nTiles);
  }
};

struct WorldPosition
{
  Vec2i absoluteChunkPos;
  Vec2i absoluteTilePos;
  Vec2f relTilePos;
};

struct BiomePoint
{
  Vec2i pos;
  Biome biome;
};

class World
{
private:
  int m_chunkSize;
  int m_nChunks;
  int m_worldSize;
  int m_nTiles;
  int m_tileSize;

  Engine *m_game;

  std::mt19937 gen;
  std::uniform_int_distribution<> biomeX;
  std::uniform_int_distribution<> biomeY;
  std::uniform_int_distribution<> biomeTypeDist;

  std::vector<Chunk *> chunks = {};
  std::vector<BiomePoint> biomePoints;

  Vec2f m_origin;
  Vec2f m_topLeft;

private:
  BiomePoint randomSample()
  {
    int bx = biomeX(gen);
    int by = biomeY(gen);
    Biome biome = static_cast<Biome>(biomeTypeDist(gen));

    return BiomePoint{Vec2i(bx, by), biome};
  }

  void generateWorld()
  {
    const int numberOfBiomesInWorld = 5;
    for (int i = 0; i < numberOfBiomesInWorld; ++i)
    {
      biomePoints.push_back(randomSample());
    }

    for (int y = 0; y < m_nChunks; ++y)
    {
      for (int x = 0; x < m_nChunks; ++x)
      {
        Biome closestBiome = Biome::Grassland;                   // defualt
        float minDistSqred = std::pow(m_nChunks * m_nChunks, 2); // Huge value at first

        // Find closest biome point
        for (const auto &point : biomePoints)
        {
          float distSqred = point.pos.distanceToSquared(Vec2i(x, y));
          if (distSqred < minDistSqred)
          {
            minDistSqred = distSqred;
            closestBiome = point.biome;
          }
        }

        chunks[y * m_nChunks + x] = new Chunk(m_nTiles, closestBiome);
      }
    }

    printWorld();
  }

  void printWorld()
  {
    for (int y = 0; y < m_nChunks; ++y)
    {
      for (int x = 0; x < m_nChunks; ++x)
      {
        auto &chunk = chunks[y * m_nChunks + x];
        char c;
        switch (chunk->closestBiome)
        {
        case Biome::Grassland:
          c = 'G';
          break;
        case Biome::Forest:
          c = 'F';
          break;
        case Biome::Rocky:
          c = 'R';
          break;
        }

        for (int i = 0; i < biomePoints.size(); ++i)
        {
          if (biomePoints[i].pos.x == x && biomePoints[i].pos.y == y)
          {
            printf("C");
            break;
          }
        }

        printf("%c\t", c);
      }
      printf("\n");
    }
  }

public:
  World(Engine *game);
  ~World()
  {
    for (auto &c : chunks)
    {
      for (auto &t : c->tiles)
      {
        if (t)
        {
          delete t;
        }
      }
      delete c;
    }
  }

  Vec2i worldToNthChunk(Vec2f worldPos);
  Vec2f worldToChunk(Vec2f worldPos);

  Vec2i worldToNthTile(Vec2f worldPos);
  Vec2f tileToWorld(Vec2i nthChunk, Vec2i nthTile);

  void setChunkTile(Vec2f worldPos, int atlasCode);
  void setChunkTile(int xthChunk, int ythChunk, int xthTile, int ythTile, int atlasCode);

  Chunk *getChunk(Vec2i pos)
  {
    // TODO Assert that this position is valid
    return chunks[pos.y * m_nChunks + pos.x];
  }

  Tile *getTileFromChunk(Chunk *c, Vec2i tilePos)
  {
    // TODO assert that this tile position is valid and "containbale type shit"
    return c->tiles[tilePos.y * m_nTiles + tilePos.x];
  }

  Tile *getTile(WorldPosition pos)
  {
    Chunk *c = getChunk(pos.absoluteChunkPos);
    Tile *t = getTileFromChunk(c, pos.absoluteTilePos);
    return t;
  }

  void drawTileGrid(int xthChunk, int ythChunk);
  void drawGrid();
  void drawChunk(int xthChunk, int ythChunk, Chunk *chunk);

  void update();
  void draw();
};
