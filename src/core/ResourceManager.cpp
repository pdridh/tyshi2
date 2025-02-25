#include "ResourceManager.h"

#include <cassert>

#include "Engine.h"

ResourceManager::ResourceManager(Engine &engine)
    : m_engine{engine}, m_nextTextureID{0}
{
}

size_t ResourceManager::loadTexture(const char *file)
{

  SDL_Texture *texture = IMG_LoadTexture(m_engine.m_renderer, file);
  if (!texture)
  {
    SDL_Log("Failed to load texture: %s, Error: %s\n", file, SDL_GetError());
    return -1;
  }

  size_t id = m_nextTextureID++;
  m_textureMap[id] = texture;
  return id;
}

SDL_Texture *ResourceManager::getTextureById(size_t id)
{
  auto it = m_textureMap.find(id);
  assert(it != m_textureMap.end());

  return it->second;
}

void ResourceManager::unloadTexture(size_t id)
{
  auto it = m_textureMap.find(id);
  assert(it != m_textureMap.end());

  SDL_DestroyTexture(it->second);
  m_textureMap.erase(id);
}

ResourceManager::~ResourceManager()
{

  auto it = m_textureMap.begin();
  while (it != m_textureMap.end())
  {
    SDL_DestroyTexture(it->second);

    ++it;
  }
}
