#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Entity.h"

class World;

class EntityManager
{
public:
  // Add a new entity and get a reference to it to change shit
  Entity &addEntity();

  // Handle removal/addition of entities separate from the modifying shit
  void refresh();

  // In case manager is destroyed before handing over queued entities to unique_ptr
  ~EntityManager();

public:
  // For adding entities gracefully without affecting updates
  std::vector<Entity *> m_addQueue;
  // All the entities
  std::vector<std::unique_ptr<Entity>> m_entities;
};