#include "World.h"

World::World(Engine *game)
    : m_game{game}, gen{555555}
{
  m_chunkSize = 256;

  m_worldSize = CHUNKS_DIM * m_chunkSize;
  m_tileSize = m_chunkSize / TILES_PER_CHUNK_DIM;

  biomeX = std::uniform_int_distribution<>(0, CHUNKS_DIM - 1);
  biomeY = std::uniform_int_distribution<>(0, CHUNKS_DIM - 1);
  biomeTypeDist = std::uniform_int_distribution<>(0, Biome::MAX_BIOME_TYPE - 1);

  m_origin = Vec2f::ZERO();

  generateWorld();

  player = new Player();
  player->pos = Vec2f(0, 0);

  player->speed = 200;
  player->textureID = 1;
}

World::~World()
{
  for (int i = 0; i < TOTAL_CHUNKS; ++i)
  {
    Chunk *&c = chunks[i];

    if (c == nullptr)
    {
      continue;
    }
    for (int j = 0; j < TOTAL_TILES_PER_CHUNK; ++j)
    {
      Tile *&t = c->tiles[j];
      if (t == nullptr)
      {
        continue;
      }

      delete t;
      t = nullptr;
    }
    delete c;
    c = nullptr;
  }

  delete player;
  player = nullptr;
}

BiomePoint World::randomSample()
{
  i32 bx = biomeX(gen);
  i32 by = biomeY(gen);
  Biome biome = static_cast<Biome>(biomeTypeDist(gen));

  return BiomePoint{Vec2i(bx, by), biome};
}

void World::generateChunkTiles(Chunk *chunky)
{
  for (int y = 0; y < TILES_PER_CHUNK_DIM; ++y)
  {
    for (int x = 0; x < TILES_PER_CHUNK_DIM; ++x)
    {
      // TODO Get random tile based on the biome
      // For now we assign all tiles 0
      // TODO ALSO change new allocation to better allocation, feel?
      Tile *t = new Tile();
      t->atlasCode = 0;
      t->nthChunk = chunky->position;
      t->position = Vec2i(x, y);
      chunky->tiles[y * TILES_PER_CHUNK_DIM + x] = t;
    }
  }
}

Chunk *World::createChunk(Vec2i pos)
{

  Biome closestBiome = Biome::Grassland;                   // defualt
  f32 minDistSqred = std::pow(CHUNKS_DIM * CHUNKS_DIM, 2); // Huge value at first

  // Find closest biome point
  for (const auto &point : biomePoints)
  {
    f32 distSqred = point.pos.distanceToSquared(pos);
    if (distSqred < minDistSqred)
    {
      minDistSqred = distSqred;
      closestBiome = point.biome;
    }
  }

  // TODO change allocation from new to maybe placement new or stackallocator type shit
  Chunk *c = new Chunk();
  c->biome = closestBiome;
  c->position = pos;
  generateChunkTiles(c);

  return c;
}

void World::generateWorld()
{
  const i32 numberOfBiomesInWorld = 5;
  for (i32 i = 0; i < numberOfBiomesInWorld; ++i)
  {
    biomePoints[i] = randomSample();
  }

  printWorld();
}

void World::printWorld()
{
  for (i32 y = 0; y < CHUNKS_DIM; ++y)
  {
    for (i32 x = 0; x < CHUNKS_DIM; ++x)
    {
      Chunk *&chunk = chunks[y * CHUNKS_DIM + x];
      if (chunk == nullptr)
      {
        continue;
      }

      printf("trying to draw chunk?\n");

      char c;
      switch (chunk->biome)
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

      for (i32 i = 0; i < NUMBER_OF_BIOME_PTS; ++i)
      {
        if (biomePoints[i].pos == Vec2i(x, y))
        {
          printf("C");
          break;
        }
      }

      printf("%c\t", c);
    }
    // printf("\n");
  }
}

Vec2i World::worldToAbsChunk(Vec2f worldPos)
{
  Vec2i nthChunk = worldPos.scaleDown(m_chunkSize);

  return nthChunk;
}

Vec2f World::absChunkToWorld(Vec2i nthChunk)
{
  Vec2f chunkPos = nthChunk.scale(m_chunkSize);
  return chunkPos;
}

Vec2i World::worldToAbsTile(Vec2f worldPos)
{
  Vec2i nthTile = worldPos.scaleDown(m_tileSize);
  return nthTile;
}

Vec2i World::worldToChunkTile(Vec2f worldPos)
{
  Vec2i absTile = worldToAbsTile(worldPos);
  Vec2i nthTile = Vec2i(absTile.x % TILES_PER_CHUNK_DIM, absTile.y % TILES_PER_CHUNK_DIM);

  return nthTile;
}

Vec2f World::absTileToWorld(Vec2i absTile)
{
  Vec2f worldPos = absTile.scale(m_tileSize) + Vec2f(m_tileSize / 2, m_tileSize / 2);
  return worldPos;
}

Vec2f World::chunkTileToWorld(Vec2i nthChunk, Vec2i nthTile)
{
  Vec2i chunkPos = nthChunk.scale(m_chunkSize);
  Vec2i tileCenter = nthTile.scale(m_tileSize) + Vec2i(m_tileSize / 2, m_tileSize / 2);
  Vec2f worldPos = chunkPos + tileCenter;

  return worldPos;
}

void World::drawGridTile(i32 xthChunk, i32 ythChunk)
{
  for (i32 ythTile = 0; ythTile < TILES_PER_CHUNK_DIM; ++ythTile)
  {
    for (i32 xthTile = 0; xthTile < TILES_PER_CHUNK_DIM; ++xthTile)
    {
      Vec2f tilePos = chunkTileToWorld(Vec2i(xthChunk, ythChunk), Vec2i(xthTile, ythTile));

      m_game->camera->drawPoint(tilePos.x, tilePos.y, {80, 80, 80, 255});
      m_game->camera->drawRect(tilePos, m_tileSize, m_tileSize, {80, 80, 80, 255});
    }
  }
}

void World::drawGrid()
{

  // For each chunk
  for (i32 ythChunk = 0; ythChunk < CHUNKS_DIM; ++ythChunk)
  {
    for (i32 xthChunk = 0; xthChunk < CHUNKS_DIM; ++xthChunk)
    {
      // Draw tiles inside chunk
      drawGridTile(xthChunk, ythChunk);
      // Draw the chunk
      m_game->camera->drawRect(Vec2f((m_chunkSize / 2) + xthChunk * m_chunkSize,
                                     ((m_chunkSize / 2) + ythChunk * m_chunkSize)),
                               m_chunkSize, m_chunkSize, {0, 255, 0, 255});
    }
  }

  // Draw world borders;
  m_game->camera->drawRect(m_origin + Vec2f(m_chunkSize * CHUNKS_DIM / 2, m_chunkSize * CHUNKS_DIM / 2), m_chunkSize * CHUNKS_DIM, m_chunkSize * CHUNKS_DIM, {255, 0, 0, 255});
}

void World::drawChunk(Chunk *chunk)
{
  int x = 0;
  for (i32 ythTile = 0; ythTile < TILES_PER_CHUNK_DIM; ++ythTile)
  {
    for (i32 xthTile = 0; xthTile < TILES_PER_CHUNK_DIM; ++xthTile)
    {
      Tile *tile = getTileFromChunk(chunk, Vec2i(xthTile, ythTile));
      if (tile)
      {
        drawTile(tile);
      }
    }
  }
}

void World::drawTile(Tile *tile)
{
  Vec2f tilePosInWorld = chunkTileToWorld(tile->nthChunk, tile->position);
  m_game->camera->drawRect(tilePosInWorld, m_tileSize - 50, m_tileSize - 50, {255, 0, 0, 255}, false);
}

void World::setChunkTile(Vec2f worldPos, i32 atlasCode)
{
  Vec2i nthChunk = worldToAbsChunk(worldPos);
  Vec2i nthTile = worldToChunkTile(worldPos);

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

Chunk *&World::getChunk(Vec2i pos)
{
  // TODO Assert that this position is valid
  Chunk *&c = chunks[pos.y * CHUNKS_DIM + pos.x];

  // The chunk requested exists
  if (c != nullptr)
  {
    return c;
  }

  // IF the chunk DOES NOT exist then we have to create it
  c = createChunk(pos);

  return c;
}

Chunk *&World::getChunkAtWorldPos(Vec2f worldPos)
{
  Vec2i nthChunk = worldToAbsChunk(worldPos);
  return getChunk(nthChunk);
}

void World::updatePlayer()
{
  Vec2f dir = Vec2f::ZERO();
  Input &input = m_game->input;

  if (input.isKeyPressed(SDL_SCANCODE_W))
  {
    dir += Vec2f(0, 1);
  }
  else if (input.isKeyPressed(SDL_SCANCODE_S))
  {
    dir += Vec2f(0, -1);
  }

  if (input.isKeyPressed(SDL_SCANCODE_A))
  {
    dir += Vec2f(-1, 0);
  }
  else if (input.isKeyPressed(SDL_SCANCODE_D))
  {
    dir += Vec2f(1, 0);
  }
  Vec2f move = dir.normalize().scale(player->speed).scale(m_game->dt);
  player->pos += move;

  m_game->camera->centerOn(player->pos);
}

void World::drawPlayer()
{

  m_game->camera->drawRect(player->pos,
                           player->squareSize, player->squareSize, {255, 0, 0, 255});
  m_game->camera->drawRect(player->pos,
                           player->squareSize / 4, player->squareSize / 4, {0, 0, 255, 255});
}

void World::update()
{
  updatePlayer();
}

void World::draw()
{
  drawGrid();
  drawPlayer();

  Chunk *&chunky = getChunkAtWorldPos(player->pos);
  drawChunk(chunky);
}