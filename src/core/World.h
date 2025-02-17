#pragma once

#include <SDL3/SDL.h>

#include "Engine.h"

#include <random>
#include <algorithm>
#include <cassert>

static constexpr int CHUNKS_DIM = 4;
static constexpr int TOTAL_CHUNKS = CHUNKS_DIM * CHUNKS_DIM;
static constexpr int NUMBER_OF_BIOME_PTS = 5;
static constexpr int TILES_PER_CHUNK_DIM = 4;
static constexpr int TOTAL_TILES_PER_CHUNK = TILES_PER_CHUNK_DIM * TILES_PER_CHUNK_DIM;

struct Tile
{
  Vec2i position;
  Vec2i nthChunk;
  i32 atlasCode;
};

struct Player
{
  Vec2f pos;
  f32 speed;
  i32 squareSize = 24;
  i32 textureID; // TODO idk how itll shape out but needs a texture fs
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
  Vec2i pos;
  Biome biome;
};

class World
{
private:
  i32 m_chunkSize;
  i32 m_worldSize;
  i32 m_tileSize;

  Engine *m_game;

  std::mt19937 gen;
  std::uniform_int_distribution<> biomeX;
  std::uniform_int_distribution<> biomeY;
  std::uniform_int_distribution<> biomeTypeDist;

  Chunk *chunks[TOTAL_CHUNKS];
  BiomePoint biomePoints[NUMBER_OF_BIOME_PTS];

  Vec2f m_origin;

  Player *player;

private:
public:
  World(Engine *game);
  ~World();

  // ***************************** Generation related stuff ****************** //

  BiomePoint randomSample();
  void generateChunkTiles(Chunk *chunky);
  Chunk *createChunk(Vec2i pos);
  void generateWorld();
  void printWorld();

  // **************************** Coordinate conversions *******************//

  // Convert from world position to a chunk (x,y)
  // not relative to anything
  Vec2i worldToAbsChunk(Vec2f worldPos);

  // Convert from a chunk(x,y) to world position
  Vec2f absChunkToWorld(Vec2i nthChunk);

  // Convert from world position to a tile not bounded by any chunk
  // THIS can range above the tile per chunk value
  Vec2i worldToAbsTile(Vec2f worldPos);

  // Convert from world position to a tile INSIDE a chunk
  // this coordinate can only range from 0,0 to the tile per chunk value
  Vec2i worldToChunkTile(Vec2f worldPos);

  // Convert absolute tile, i.e, a tile unbounded by any chunk to world position
  Vec2f absTileToWorld(Vec2i absTile);

  // Convert a chunk(x,y)'s tile(xt, yt) to a world position
  Vec2f chunkTileToWorld(Vec2i nthChunk, Vec2i nthTile);

  void setChunkTile(Vec2f worldPos, i32 atlasCode);
  void setChunkTile(i32 xthChunk, i32 ythChunk, i32 xthTile, i32 ythTile, i32 atlasCode);

  // Get the reference to the ptr in chunks[]
  // by the absolute (x,y) of a chunk
  // If a chunk exists it returns it otherwise
  // creates the chunk using createChunk() and returns it
  Chunk *&getChunk(Vec2i nthChunk);

  // Get reference to the ptr in chunks[] directly by
  // a worldPos, converts the worldPos to an absolute chunk
  // and calls getChunk() internally (meaning this can create a chunk too)
  Chunk *&getChunkAtWorldPos(Vec2f worldPos);

  Tile *getTileFromChunk(Chunk *c, Vec2i tilePos)
  {

    assert(tilePos.x >= 0 &&
           tilePos.y >= 0 &&
           tilePos.x < TILES_PER_CHUNK_DIM &&
           tilePos.y < TILES_PER_CHUNK_DIM);

    return c->tiles[tilePos.y * TILES_PER_CHUNK_DIM + tilePos.x];
  }

  void drawGridTile(i32 xthChunk, i32 ythChunk);
  void drawGrid();

  void drawChunk(Chunk *chunk);
  void drawTile(Tile *tile);

  // Player stuff
  void updatePlayer();
  void drawPlayer();

  void update();
  void draw();
};
