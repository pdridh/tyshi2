#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include <SDL2/SDL.h>

#include "Entity.h"

class EntityManager
{
public:
  // Add a new entity and get a reference to it to change shit
  Entity &addEntity();

  // Call the update of each entity
  void update(const double dt);

  // Call the draw of each entity
  void draw(SDL_Renderer *renderer);

  // Handle removal/addition of entities separate from the updating/other modifying shit
  void refresh();

  // In case manager is destroyed before handing over queued entities to unique_ptr
  ~EntityManager();

public:
  // For adding entities gracefully without affecting updates
  std::vector<Entity *> m_addQueue;
  // All the entities
  std::vector<std::unique_ptr<Entity>> m_entities;
};