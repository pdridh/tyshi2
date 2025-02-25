#pragma once

#include <SDL3/SDL.h>

#include "Engine.h"
#include "../utils/WorldPosition.h"

#include <random>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <string>

static constexpr int CHUNKS_DIM = 16;
static constexpr int TOTAL_CHUNKS = CHUNKS_DIM * CHUNKS_DIM;
static constexpr int NUMBER_OF_BIOME_PTS = 5;
static constexpr int TILES_PER_CHUNK_DIM = 16;
static constexpr int TOTAL_TILES_PER_CHUNK = TILES_PER_CHUNK_DIM * TILES_PER_CHUNK_DIM;

struct Tile
{
  Vec2i position;
  Vec2i nthChunk;
  i32 atlasCode;
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
  Vec2i position;
  Biome biome;
  Tile *tiles[TOTAL_TILES_PER_CHUNK];
};

struct BiomePoint
{
  Vec2i position;
  Biome biome;
};

static size_t nextTextureId = 1;

class World
{
public:
  u8 m_renderDistance;
  i32 m_chunkSize;
  i32 m_worldSize;
  i32 m_tileSize;

  Engine &m_engine;

  std::mt19937 gen;
  std::uniform_int_distribution<> biomeX;
  std::uniform_int_distribution<> biomeY;
  std::uniform_int_distribution<> biomeTypeDist;

  Chunk *chunks[TOTAL_CHUNKS];
  BiomePoint biomePoints[NUMBER_OF_BIOME_PTS];

  Vec2f m_origin;

private:
public:
  World(Engine &engine);
  ~World();

  // ***************************** Generation related stuff ****************** //

  BiomePoint randomSample();
  void generateChunkTiles(Chunk *chunky);
  Chunk *createChunk(Vec2i position);
  void generateWorld();
  void printWorld();

  // **************************** Coordinate conversions *******************//

  // Convert from actual position to a chunk (x,y)
  // not relative to anything
  Vec2i actualToAbsChunk(Vec2f actualPosition);

  // Convert from a chunk(x,y) to actual position
  // The actual position is just the start of the chunk (bottom left)
  Vec2f absChunkToActual(Vec2i nthChunk);

  // Convert from acutal position to a tile not bounded by any chunk
  // THIS can range above the tile per chunk value
  Vec2i actualToAbsTile(Vec2f actualPosition);

  // Convert from actual position to a tile INSIDE a chunk
  // the returned coordinate can only range from 0,0 to the tile per chunk value
  Vec2i actualToChunkTile(Vec2f actualPosition);

  // Convert absolute tile, i.e, a tile unbounded by any chunk to actual position
  // The actual position returned is centered at the middle of the tile
  Vec2f absTileToActual(Vec2i absTile);

  // Convert a chunk(x,y)'s tile(xt, yt) to an actual position
  // The actual position is returned at the center of the tile
  Vec2f chunkTileToActual(Vec2i nthChunk, Vec2i nthTile);

  // Get the WorldPosition (chunk, tile and actual) from an actual position
  // Calls actualToAbsChunk() and actualToChunkTile() internally
  WorldPosition actualToWorld(Vec2f actualPosition);

  // Get the WorldPosition from an absolute chunk position
  // The position is set at the bottom left of the chunk
  WorldPosition absChunkToWorld(Vec2i nthChunk);

  // Get the WorldPosition from an absolute chunk position and the tile inside that chunk
  WorldPosition chunkTileToWorld(Vec2i nthChunk, Vec2i nthTile);

  // **************************** Data retrieving stuff *******************//

  // Get the reference to the ptr in chunks[]
  // by the absolute (x,y) of a chunk
  // If a chunk exists it returns it otherwise
  // creates the chunk using createChunk() and returns it
  Chunk *&getChunk(Vec2i nthChunk);

  // Get reference to the ptr in chunks[] directly by
  // a actualPosition, converts the actualPosition to an absolute chunk
  // and calls getChunk() internally (meaning this can create a chunk too)
  Chunk *&getChunkAtActual(Vec2f actualPos);

  // Just a handy wrapper that retrieves the tile from a chunk's
  // 1D array
  Tile *getTileFromChunk(Chunk *c, Vec2i nthTile);

  // **************************** Updating  stuff *******************//

  void update();

  // **************************** Drawing stuff *******************//

  void drawGridTile(i32 xthChunk, i32 ythChunk);
  void drawGrid();

  // Draw chunk based on its chunk position
  void drawChunk(Vec2i nthChunk);

  // Draw a chunk given its ptr
  void drawChunk(Chunk *chunk);
  void drawTile(Tile *tile);
  void draw();
};
