#include "World.h"

World::World(Engine *game)
    : m_game{game}
{
  m_chunkSize = 512;
  m_nChunks = 8;
  m_nTiles = 8;

  m_worldSize = m_nChunks * m_chunkSize;
  m_tileSize = m_chunkSize / m_nTiles;

  m_origin = Vec2::ZERO();
  m_topLeft = m_origin - Vec2(m_worldSize / 2, (m_worldSize / 2) * -1);

  // Storing 2d in 1d
  chunks.resize(m_nChunks * m_nChunks);

  tileSheet = IMG_LoadTexture(m_game->m_renderer, "assets/tileset.png");
  int tileWidth = 16;
  int tileHeight = 16;
  int texWidth;
  int texHeight;

  SDL_QueryTexture(tileSheet, NULL, NULL, &texWidth, &texHeight);

  // Extract
  int xTiles = texWidth / tileWidth;
  int yTiles = texHeight / tileHeight;

  for (int y = 0; y < yTiles; ++y)
  {
    for (int x = 0; x < xTiles; ++x)
    {
      SDL_Rect tile = {x * tileWidth, y * tileHeight, tileWidth, tileHeight};
      atlas.push_back(tile);
    }
  }

  for (int i = 0; i < atlas.size(); ++i)
  {
    printf("**********ATLAS CODE******: %d\n", i);
    printf("Tile info: %d, %d\n", atlas[i].x, atlas[i].y);
  }
}

Vec2 World::worldToNthChunk(Vec2 worldPos)
{
  // Change origin to world top left
  Vec2 worldTopLeftPos = worldPos - m_topLeft;
  worldTopLeftPos = Vec2(fabs(worldTopLeftPos.x), fabs(worldTopLeftPos.y));

  Vec2 nthChunk = worldTopLeftPos.getScaled(1.f / m_chunkSize);

  return nthChunk;
}

Vec2 World::worldToChunk(Vec2 worldPos)
{

  Vec2 nthChunk = worldToNthChunk(worldPos);

  Vec2 worldTopLeftPos = worldPos - m_topLeft;
  worldTopLeftPos = Vec2(fabs(worldTopLeftPos.x), fabs(worldTopLeftPos.y));

  // TODO change the casts to Vec2Integers
  Vec2 chunkOrigin = Vec2((int)nthChunk.x * m_chunkSize, (int)nthChunk.y * m_chunkSize);
  Vec2 chunkPos = worldTopLeftPos - chunkOrigin;

  return chunkPos;
}

Vec2 World::worldToNthTile(Vec2 worldPos)
{
  Vec2 chunkPos = worldToChunk(worldPos);
  Vec2 nthTile = chunkPos.getScaled(1.f / m_tileSize);

  return nthTile;
}

Vec2 World::tileToWorld(Vec2 nthChunk, Vec2 nthTile)
{
  // nthTile.scale(m_tileSize);
  Vec2 chunkTopLeft = nthChunk.scale(m_chunkSize);
  Vec2 tileCenter = nthTile.scale(m_tileSize) + Vec2(m_tileSize / 2, m_tileSize / 2);
  Vec2 worldPos = Vec2(m_topLeft.x, -1 * m_topLeft.y) + chunkTopLeft + tileCenter;

  return worldPos;
}

void World::drawTileGrid(int xthChunk, int ythChunk)
{
  for (int ythTile = 0; ythTile < m_nTiles; ++ythTile)
  {
    for (int xthTile = 0; xthTile < m_nTiles; ++xthTile)
    {
      Vec2 tilePos = tileToWorld(Vec2(xthChunk, ythChunk), Vec2(xthTile, ythTile));
      m_game->camera->drawRect(tilePos, m_tileSize, m_tileSize, {80, 80, 80, 255});
    }
  }
}

void World::drawGrid()
{

  // For each chunk
  for (int ythChunk = 0; ythChunk < m_nChunks; ++ythChunk)
  {
    for (int xthChunk = 0; xthChunk < m_nChunks; ++xthChunk)
    {
      // Draw tiles inside chunk
      drawTileGrid(xthChunk, ythChunk);
      // Draw the chunk
      m_game->camera->drawRect(Vec2(m_topLeft.x + m_chunkSize / 2 + (xthChunk * m_chunkSize),
                                    -1 * m_topLeft.y + m_chunkSize / 2 + (ythChunk * m_chunkSize)),
                               (float)m_chunkSize, (float)m_chunkSize, {0, 255, 0, 255});
    }
  }

  // Draw world borders;
  m_game->camera->drawRect(m_origin, m_chunkSize * m_nChunks, m_chunkSize * m_nChunks, {255, 0, 0, 255});
}

void World::drawChunk(int xthChunk, int ythChunk, Chunk *chunk)
{

  for (int ythTile = 0; ythTile < m_nTiles; ++ythTile)
  {
    for (int xthTile = 0; xthTile < m_nTiles; ++xthTile)
    {
      if (chunk->tiles[ythTile * m_nTiles + xthTile])
      {
        Vec2 tilePos =
            tileToWorld(Vec2(xthChunk, ythChunk), Vec2(xthTile, ythTile));
        int tileCode = chunk->tiles[ythTile * m_nTiles + xthTile]->atlasCode;
        m_game->camera->drawTexture(tileSheet, atlas[tileCode], tilePos, m_tileSize, m_tileSize);
      }
    }
  }
}

void World::changeChunkTile(Vec2 worldPos, int atlasCode)
{
  Vec2 nthChunk = worldToNthChunk(worldPos);
  Vec2 chunkPos = worldToChunk(worldPos);
  Vec2 nthTile = worldToNthTile(worldPos);

  // TODO change to Vec2I instead of casting
  changeChunkTile((int)nthChunk.x, (int)nthChunk.y, (int)nthTile.x, (int)nthTile.y, atlasCode);
}

void World::changeChunkTile(int xthChunk, int ythChunk, int xthTile, int ythTile, int atlasCode)
{
  printf("Changing tile at: CHUNK: %d, %d; TILE: %d,%d to %d\n", xthChunk, ythChunk, xthTile, ythTile, atlasCode);
  auto &chunk = chunks[ythChunk * m_nChunks + xthChunk];
  if (chunk == nullptr)
  {
    chunk = std::make_unique<Chunk>(m_nTiles);
    chunk->tiles[ythTile * m_nTiles + xthTile] = std::make_unique<Tile>(atlasCode);
    return;
  }

  auto &tile = chunk->tiles[ythTile * m_nTiles + xthTile];

  // Check if tile is created
  if (tile == nullptr)
  {
    tile = std::make_unique<Tile>(atlasCode);
    return;
  }

  tile->atlasCode = atlasCode;
}

void World::update()
{
}

void World::draw()
{
  // for (const auto &layer : layers)
  // {

  drawGrid();

  for (int y = 0; y < m_nChunks; ++y)
  {
    for (int x = 0; x < m_nChunks; ++x)
    {
      if (chunks[y * m_nChunks + x])
      {
        drawChunk(x, y, chunks[y * m_nChunks + x].get());
      }
    }
  }
}