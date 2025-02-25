#pragma once

#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Engine;

class ResourceManager
{
private:
  Engine &m_engine;
  std::unordered_map<size_t, SDL_Texture *> m_textureMap;
  size_t m_nextTextureID;

public:
  ResourceManager(Engine &engine);

  size_t loadTexture(const char *file);
  SDL_Texture *getTextureById(size_t id);
  void unloadTexture(size_t id);

  ~ResourceManager();
};

#define ResourceManger ResourceManager::instance()
