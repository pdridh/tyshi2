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
  virtual void update(const double dt) = 0;
  virtual void draw(SDL_Renderer *renderer) = 0;
  virtual ~Component() {}

protected:
  Entity *m_parent;
};