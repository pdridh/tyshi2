#pragma once

class Entity;

struct Component
{
public:
  Component(Entity *parent)
      : m_parent{parent}
  {
  }
  virtual ~Component() = default;

protected:
  Entity *m_parent;
};