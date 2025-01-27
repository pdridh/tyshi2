#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Engine.hpp"

#include <memory>
#include <vector>

struct Tile
{
  int atlasCode;
  Tile(int code = -1) : atlasCode{code} {}
};

struct Chunk
{

  std::vector<std::unique_ptr<Tile>> tiles;

  Chunk(int nTiles)
  {
    // Storing 2d in 1d
    tiles.resize(nTiles * nTiles);
  }
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

  std::vector<std::unique_ptr<Chunk>> chunks = {};
  SDL_Texture *tileSheet;
  std::vector<SDL_Rect> atlas;

  Vec2f m_origin;
  Vec2f m_topLeft;

public:
  World(Engine *game);

  Vec2i worldToNthChunk(Vec2f worldPos);
  Vec2f worldToChunk(Vec2f worldPos);

  Vec2i worldToNthTile(Vec2f worldPos);
  Vec2f tileToWorld(Vec2i nthChunk, Vec2i nthTile);

  void drawTileGrid(int xthChunk, int ythChunk);
  void drawGrid();
  void drawChunk(int xthChunk, int ythChunk, Chunk *chunk);
  void changeChunkTile(Vec2f worldPos, int atlasCode);
  void changeChunkTile(int xthChunk, int ythChunk, int xthTile, int ythTile, int atlasCode);

  void update();
  void draw();
};