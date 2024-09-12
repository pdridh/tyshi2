#include "EntityManager.h"

Entity &EntityManager::addEntity()
{
  Entity *e(new Entity(*this));
  m_addQueue.push_back(e);
  return *e;
}

void EntityManager::update(const double dt)
{
  for (auto &e : m_entities)
  {
    e->update(dt);
  }
}

void EntityManager::draw(SDL_Renderer *renderer)
{
  for (auto &e : m_entities)
  {
    e->draw(renderer);
  }
}

void EntityManager::refresh()
{
  // Add new entities
  for (auto newEntity : m_addQueue)
  {
    m_entities.emplace_back(std::move(newEntity));
  }
  m_addQueue.clear();

  m_entities.erase(
      std::remove_if(std::begin(m_entities), std::end(m_entities),
                     [](const std::unique_ptr<Entity> &entity)
                     {
                       return !entity->isAlive();
                     }),
      std::end(m_entities));
}

EntityManager::~EntityManager()
{
  if (m_addQueue.empty())
  {
    return;
  }

  for (auto e : m_addQueue)
  {
    delete e;
    e = nullptr;
  }

  m_addQueue.clear();
}
