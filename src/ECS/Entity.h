#pragma once

#include <cassert>

#include "ECS.h"
#include "components/Component.h"
#include "components/Transform.h"

class Entity;
class EntityManager;

class Entity
{
public:
  // Returns true if the component has been registered false otherwise
  template <typename ComponentType>
  bool hasComponent() const
  {
    return m_components[getComponentTypeID<ComponentType>()] != nullptr;
  }

  // Add a component
  // Returns a reference to the component that was added
  template <typename ComponentType, typename... CompArgs>
  inline ComponentType &addComponent(CompArgs &&...args)
  {

    // The component should be null to add
    // otherwise this component has already been added
    assert(m_components[getComponentTypeID<ComponentType>()] == nullptr);

    ComponentType *c = new ComponentType(this, std::forward<CompArgs>(args)...);
    m_components[getComponentTypeID<ComponentType>()] = c;

    return *c;
  }

  template <typename ComponentType>
  inline ComponentType &getComponent()
  {
    size_t id = getComponentTypeID<ComponentType>();
    assert(m_components[id] != nullptr);

    // First cast the generic ptr to the component into
    // the component thats being requested
    // Then return THE REFERENCE of the component
    return *static_cast<ComponentType *>(m_components[id]);
  }

  template <typename ComponentType>
  void removeComponent()
  {
    size_t id = getComponentTypeID<ComponentType>();
    // The component has to be present if u wanna remove it
    assert(m_components[id] != nullptr);

    delete m_components[id];
    m_components[id] = nullptr;
  }

  void update(const double dt)
  {
    for (auto &c : m_components)
    {
      if (c)
      {
        c->update(dt);
      }
    }
  }

  void draw(SDL_Renderer *renderer)
  {
    for (auto &c : m_components)
    {
      if (c)
      {
        c->draw(renderer);
      }
    }
  }

  bool isAlive() const
  {
    return alive;
  }

  // Flag self as inactive so entitymanager cleans it up
  void die()
  {
    alive = false;
  }

  ~Entity()
  {
    // Clean up components
    //  i tried using <memory> but fuck allat
    for (auto c : m_components)
    {
      delete c;
      c = nullptr;
    }

    m_components.clear();
  }

private:
  // Private constructor cause only manager can create entities
  Entity(EntityManager &manager)
      : m_manager{manager}
  {
    // Resize the vector to the number of components that can be held by an ent
    // Unpacked so lots of cache misses but dont give a fuck cause optimization later
    //  I cant event get the player to fucking do an action except walking rn so fuck off
    m_components.resize(MAX_COMPONENTS);
    // All components have a transform component in creation
    addComponent<Transform>();
    alive = true;
  }
  friend class EntityManager;

private:
  // Flag marking self as destroyable for the EntityManager
  bool alive;

  // Reference to the manager
  EntityManager &m_manager;

  // Lots of cache hits but we DO NOT care I REPEAT WE DO NOT GIVE A SHIT OPTIMIZATION LATEEERRRR
  std::vector<Component *> m_components;
};
