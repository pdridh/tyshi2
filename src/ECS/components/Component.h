#pragma once

#include <SDL2/SDL.h>

class Entity;

class Component
{
public:
  Component(Entity *parent)
      : m_parent{parent}
  {
  }
  virtual void update(const double dt) {}
  virtual void draw(SDL_Renderer *renderer) {}
  virtual ~Component() {}

protected:
  Entity *m_parent;
};