#pragma once

#include <vector>
#include <iostream>

class Entity;
class Component;

// For assigning new ids to each component type
inline size_t getNextID()
{
  static size_t id = 0;
  return id++;
}

// For assigning new ids to each component type
template <typename ComponentType>
inline size_t getComponentTypeID() noexcept
{
  static_assert(std::is_base_of<Component, ComponentType>::value, "Type provided is not a Component");
  static size_t id = getNextID();
  return id;
}

constexpr size_t MAX_COMPONENTS = 32;