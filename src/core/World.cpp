#include "World.h"

World::World(Engine *game)
    : m_game{game}, gen{555555}
{
  m_chunkSize = 256;

  m_worldSize = CHUNKS_DIM * m_chunkSize;
  m_tileSize = m_chunkSize / TILES_PER_CHUNK_DIM;
  m_renderDistance = 3;

  biomeX = std::uniform_int_distribution<>(0, CHUNKS_DIM - 1);
  biomeY = std::uniform_int_distribution<>(0, CHUNKS_DIM - 1);
  biomeTypeDist = std::uniform_int_distribution<>(0, Biome::MAX_BIOME_TYPE - 1);

  m_origin = Vec2f::ZERO();

  generateWorld();

  player = new Player();
  player->position = absChunkToWorld(Vec2i(0, 0));

  player->speed = 500;
  player->textureID = loadTexture("assets/char_walk.png");
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

Chunk *World::createChunk(Vec2i position)
{

  Biome closestBiome = Biome::Grassland;                   // defualt
  f32 minDistSqred = std::pow(CHUNKS_DIM * CHUNKS_DIM, 2); // Huge value at first

  // Find closest biome point
  for (const auto &point : biomePoints)
  {
    f32 distSqred = point.position.distanceToSquared(position);
    if (distSqred < minDistSqred)
    {
      minDistSqred = distSqred;
      closestBiome = point.biome;
    }
  }

  // TODO change allocation from new to maybe placement new or stackallocator type shit
  Chunk *chunky = new Chunk();
  chunky->biome = closestBiome;
  chunky->position = position;
  generateChunkTiles(chunky);


  return chunky;
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
  // TODO also some way to store and retrieve THE WORLD SEEDD
  // Prints the biome points with their psoitions
  for (auto &point : biomePoints)
  {
    point.position.print("Biome pt");

    switch (point.biome)
    {
    case Grassland:
      printf("Grass\n");
      break;
    case Forest:
      printf("Forest\n");
      break;
    case Rocky:
      printf("Rocky\n");
      break;
    }
  }
}

Vec2i World::actualToAbsChunk(Vec2f actualPosition)
{
  Vec2i nthChunk = actualPosition.scaleDown(m_chunkSize);
  return nthChunk;
}

Vec2f World::absChunkToActual(Vec2i nthChunk)
{
  Vec2f actualPosition = nthChunk.scale(m_chunkSize);
  return actualPosition;
}

Vec2i World::actualToAbsTile(Vec2f actualPosition)
{
  Vec2i nthTile = actualPosition.scaleDown(m_tileSize);
  return nthTile;
}

Vec2i World::actualToChunkTile(Vec2f actualPosition)
{
  Vec2i absTile = actualToAbsTile(actualPosition);
  Vec2i nthTile = Vec2i(absTile.x % TILES_PER_CHUNK_DIM, absTile.y % TILES_PER_CHUNK_DIM);

  return nthTile;
}

Vec2f World::absTileToActual(Vec2i nthTile)
{
  // Centered
  Vec2f actualPosition = nthTile.scale(m_tileSize) + Vec2f(m_tileSize / 2, m_tileSize / 2);
  return actualPosition;
}

Vec2f World::chunkTileToActual(Vec2i nthChunk, Vec2i nthTile)
{
  Vec2f chunkPosition = nthChunk.scale(m_chunkSize);
  Vec2i tileCenter = nthTile.scale(m_tileSize) + Vec2i(m_tileSize / 2, m_tileSize / 2);
  Vec2f actualPosition = chunkPosition + tileCenter;

  return actualPosition;
}

WorldPosition World::actualToWorld(Vec2f actualPosition)
{
  Vec2i nthChunk = actualToAbsChunk(actualPosition);
  Vec2i nthTile = actualToChunkTile(actualPosition);

  return WorldPosition{nthChunk, nthTile, actualPosition};
}

WorldPosition World::absChunkToWorld(Vec2i nthChunk)
{
  Vec2i nthTile = Vec2i(0, 0);
  Vec2f actualPosition = absChunkToActual(nthChunk);

  return WorldPosition{nthChunk, nthTile, actualPosition};
}

WorldPosition World::chunkTileToWorld(Vec2i nthChunk, Vec2i nthTile)
{
  Vec2f actualPosition = chunkTileToActual(nthChunk, nthTile);

  return WorldPosition{nthChunk, nthTile, actualPosition};
}

Chunk *&World::getChunk(Vec2i position)
{
  assert(position.x >= 0 && position.y >= 0 && position.x < CHUNKS_DIM && position.y < CHUNKS_DIM);
  Chunk *&c = chunks[position.y * CHUNKS_DIM + position.x];

  // The chunk requested exists
  if (c != nullptr)
  {
    return c;
  }

  // IF the chunk DOES NOT exist then we have to create it
  c = createChunk(position);
  return c;
}

Chunk *&World::getChunkAtActual(Vec2f actualPosition)
{
  Vec2i nthChunk = actualToAbsChunk(actualPosition);
  return getChunk(nthChunk);
}

Tile *World::getTileFromChunk(Chunk *c, Vec2i nthTile)
{
  assert(nthTile.x >= 0 &&
         nthTile.y >= 0 &&
         nthTile.x < TILES_PER_CHUNK_DIM &&
         nthTile.y < TILES_PER_CHUNK_DIM);

  return c->tiles[nthTile.y * TILES_PER_CHUNK_DIM + nthTile.x];
}

void World::updatePlayer()
{
  Vec2f dir = Vec2f::ZERO();
  Input &input = m_game->input;

  Vec2f oldPosition = player->position.actualPosition;

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
  Vec2f newPosition = oldPosition + move;

  // TODO restrain player somehow idk
  if (newPosition.x < 0)
  {
    newPosition.x = 0;
  }

  player->position = actualToWorld(newPosition);
  m_game->camera->centerOn(player->position.actualPosition);
}

void World::update()
{
  updatePlayer();
}

void World::drawGridTile(i32 xthChunk, i32 ythChunk)
{
  for (i32 ythTile = 0; ythTile < TILES_PER_CHUNK_DIM; ++ythTile)
  {
    for (i32 xthTile = 0; xthTile < TILES_PER_CHUNK_DIM; ++xthTile)
    {
      Vec2f tilePosition = chunkTileToActual(Vec2i(xthChunk, ythChunk), Vec2i(xthTile, ythTile));

      m_game->camera->drawPoint(tilePosition.x, tilePosition.y, {80, 80, 80, 255});
      m_game->camera->drawRect(tilePosition, m_tileSize, m_tileSize, {80, 80, 80, 255});
    }
  }
}

void World::drawGrid()
{

  Vec2i playerChunk = player->position.nthChunk;

  // For each chunk
  for (i32 ythChunk = -m_renderDistance; ythChunk <= m_renderDistance; ++ythChunk)
  {
    for (i32 xthChunk = -m_renderDistance; xthChunk <= m_renderDistance; ++xthChunk)
    {
      Vec2i renderChunk = playerChunk + Vec2i(xthChunk, ythChunk);

      if (renderChunk.x >= CHUNKS_DIM ||
          renderChunk.y >= CHUNKS_DIM ||
          renderChunk.x < 0 ||
          renderChunk.y < 0)
        continue;

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

void World::drawChunk(Vec2i nthChunk)
{
  Chunk *&chunky = getChunk(nthChunk);
  drawChunk(chunky);
}

void World::drawChunk(Chunk *chunk)
{
  for (i32 ythTile = 0; ythTile < TILES_PER_CHUNK_DIM; ++ythTile)
  {
    for (i32 xthTile = 0; xthTile < TILES_PER_CHUNK_DIM; ++xthTile)
    {
      Tile *tile = getTileFromChunk(chunk, Vec2i(xthTile, ythTile));
      if (tile)
      {
        Vec2f tilePosition = chunkTileToActual(tile->nthChunk, tile->position);
        Color biomeColor;
        switch (chunk->biome)
        {
        case Biome::Grassland:
          biomeColor = {0, 255, 0, 255};
          break;
        case Biome::Forest:
          biomeColor = {0, 128, 12, 255};
          break;
        case Biome::Rocky:
          biomeColor = {123, 63, 0, 255};
          break;
        }
        m_game->camera->drawRect(tilePosition, m_tileSize, m_tileSize, biomeColor, true);
      }
    }
  }
}

void World::drawTile(Tile *tile)
{
  Vec2f tilePosition = chunkTileToActual(tile->nthChunk, tile->position);
  SDL_FRect srcRect = {4 * 16, 8 * 16, 16, 16};
}

void World::drawPlayer()
{

  SDL_FRect src = {0, 0, 32, 32};
  m_game->camera->drawTexture(player->position.actualPosition, getTextureById(player->textureID), src,
                              player->squareSize * 4, player->squareSize * 4, true);
}

void World::draw()
{
  // Draw all the chunks in the render distance
  Vec2i playerChunk = player->position.nthChunk;
  for (int y = -m_renderDistance; y <= m_renderDistance; y++)
  {
    for (int x = -m_renderDistance; x <= m_renderDistance; x++)
    {
      Vec2i renderChunk = playerChunk + Vec2i(x, y);

      if (renderChunk.x >= CHUNKS_DIM || renderChunk.y >= CHUNKS_DIM || renderChunk.x < 0 || renderChunk.y < 0)
        continue;

      drawChunk(renderChunk);
    }
  }

  drawGrid();
  drawPlayer();
}