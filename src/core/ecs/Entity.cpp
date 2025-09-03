#include "Entity.h"

bool Entity::isAlive() const
{
  return alive;
}

void Entity::die()
{
  alive = false;
}

Entity::Entity(EntityManager &manager)
    : m_manager{manager}
{
  // Resize the vector to the number of components that can be held by an ent
  // Unpacked so lots of cache misses but dont give a fuck cause optimization later
  // I cant event get the player to fucking do an action except walking rn so fuck off
  m_components.resize(MAX_COMPONENTS);
  // All components have a transform component in creation
  addComponent<Transform>();
  alive = true;
}