#include "World.h"

World::World(Engine *game)
    : m_game{game}, gen{555555}
{
  m_chunkSize = 512;
  m_nChunks = 8;
  m_nTiles = 8;

  m_worldSize = m_nChunks * m_chunkSize;
  m_tileSize = m_chunkSize / m_nTiles;

  biomeX = std::uniform_int_distribution<>(0, m_nChunks - 1);
  biomeY = std::uniform_int_distribution<>(0, m_nChunks - 1);
  biomeTypeDist = std::uniform_int_distribution<>(0, Biome::MAX_BIOME_TYPE - 1);

  m_origin = Vec2f::ZERO();
  m_topLeft = m_origin - Vec2f(m_worldSize / 2, (m_worldSize / 2) * -1);

  // Storing 2d in 1d
  chunks.resize(m_nChunks * m_nChunks);

  generateWorld();
}

Vec2i World::worldToNthChunk(Vec2f worldPos)
{
  // Change origin to world top left
  Vec2f worldTopLeftPos = worldPos - m_topLeft;

  worldTopLeftPos = Vec2f(std::fabs(worldTopLeftPos.x), std::fabs(worldTopLeftPos.y));

  Vec2i nthChunk = worldTopLeftPos.scaleDown(m_chunkSize);

  return nthChunk;
}

Vec2f World::worldToChunk(Vec2f worldPos)
{

  Vec2i nthChunk = worldToNthChunk(worldPos);

  Vec2f worldTopLeftPos = worldPos - m_topLeft;
  worldTopLeftPos = Vec2f(fabs(worldTopLeftPos.x), fabs(worldTopLeftPos.y));

  Vec2f chunkOrigin = nthChunk.scale(m_chunkSize);
  Vec2f chunkPos = worldTopLeftPos - chunkOrigin;

  return chunkPos;
}

Vec2i World::worldToNthTile(Vec2f worldPos)
{
  Vec2f chunkPos = worldToChunk(worldPos);
  Vec2i nthTile = chunkPos.scaleDown(m_tileSize);

  return nthTile;
}

Vec2f World::tileToWorld(Vec2i nthChunk, Vec2i nthTile)
{
  Vec2i chunkTopLeft = nthChunk.scale(m_chunkSize);
  Vec2i tileCenter = nthTile.scale(m_tileSize) + Vec2i(m_tileSize / 2, m_tileSize / 2);
  Vec2f worldPos = Vec2i(m_topLeft.x, -1 * m_topLeft.y) + chunkTopLeft + tileCenter;

  return worldPos;
}

void World::drawTileGrid(i32 xthChunk, i32 ythChunk)
{
  for (i32 ythTile = 0; ythTile < m_nTiles; ++ythTile)
  {
    for (i32 xthTile = 0; xthTile < m_nTiles; ++xthTile)
    {
      Vec2f tilePos = tileToWorld(Vec2i(xthChunk, ythChunk), Vec2i(xthTile, ythTile));
      m_game->camera->drawRect(tilePos, m_tileSize, m_tileSize, {80, 80, 80, 255});
    }
  }
}

void World::drawGrid()
{

  // For each chunk
  for (i32 ythChunk = 0; ythChunk < m_nChunks; ++ythChunk)
  {
    for (i32 xthChunk = 0; xthChunk < m_nChunks; ++xthChunk)
    {
      // Draw tiles inside chunk
      drawTileGrid(xthChunk, ythChunk);
      // Draw the chunk
      m_game->camera->drawRect(Vec2f(m_topLeft.x + m_chunkSize / 2 + (xthChunk * m_chunkSize),
                                     -1 * m_topLeft.y + m_chunkSize / 2 + (ythChunk * m_chunkSize)),
                               m_chunkSize, m_chunkSize, {0, 255, 0, 255});
    }
  }

  // Draw world borders;
  m_game->camera->drawRect(m_origin, m_chunkSize * m_nChunks, m_chunkSize * m_nChunks, {255, 0, 0, 255});
}

void World::drawChunk(i32 xthChunk, i32 ythChunk, Chunk *chunk)
{

  for (i32 ythTile = 0; ythTile < m_nTiles; ++ythTile)
  {
    for (i32 xthTile = 0; xthTile < m_nTiles; ++xthTile)
    {
      if (chunk->tiles[ythTile * m_nTiles + xthTile])
      {
        // Vec2f tilePos =
        //     tileToWorld(Vec2i(xthChunk, ythChunk), Vec2i(xthTile, ythTile));
        // i32 tileCode = chunk->tiles[ythTile * m_nTiles + xthTile]->atlasCode;
        // m_game->camera->drawTexture(tileSheet, atlas[tileCode], tilePos, m_tileSize, m_tileSize);
      }
    }
  }
}

void World::setChunkTile(Vec2f worldPos, i32 atlasCode)
{
  Vec2i nthChunk = worldToNthChunk(worldPos);
  Vec2i nthTile = worldToNthTile(worldPos);

  setChunkTile(nthChunk.x, nthChunk.y, nthTile.x, nthTile.y, atlasCode);
}

void World::setChunkTile(i32 xthChunk, i32 ythChunk, i32 xthTile, i32 ythTile, i32 atlasCode)
{
  // auto &chunk = chunks[ythChunk * m_nChunks + xthChunk];
  // if (chunk == nullptr)
  // {
  //   chunk = std::make_unique<Chunk>(m_nTiles);
  //   chunk->tiles[ythTile * m_nTiles + xthTile] = std::make_unique<Tile>(atlasCode);
  //   return;
  // }

  // auto &tile = chunk->tiles[ythTile * m_nTiles + xthTile];

  // // Check if tile is created
  // if (tile == nullptr)
  // {
  //   tile = std::make_unique<Tile>(atlasCode);
  //   return;
  // }

  // tile->atlasCode = atlasCode;
}

void World::update()
{
}

void World::draw()
{
  // for (const auto &layer : layers)
  // {

  drawGrid();

  for (i32 y = 0; y < m_nChunks; ++y)
  {
    for (i32 x = 0; x < m_nChunks; ++x)
    {
      if (chunks[y * m_nChunks + x])
      {
        drawChunk(x, y, chunks[y * m_nChunks + x]);
      }
    }
  }
}