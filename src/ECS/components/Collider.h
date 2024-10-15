#pragma once

#include <SDL2/SDL.h>

#include "Component.h"
#include "../../utils/Vec2.h"

class Collider : public Component
{
public:
  Collider(Entity *parent, float width, float height)
      : Component{parent}, box{0, 0, width, height}
  {
    updatePosition();
  }

  void update(const double dt)
  {
    updatePosition();
  }

  void draw(SDL_Renderer *renderer)
  {
    if (debug)
    {
      SDL_SetRenderDrawColor(renderer, debugColor.r, debugColor.g, debugColor.b, debugColor.a);
      SDL_RenderDrawRectF(renderer, &box);
    }
  }

private:
  void updatePosition()
  {
    Vec2 pos = m_parent->getComponent<Transform>().position;
    box.x = (float)(pos.x - box.w / 2);
    box.y = (float)(pos.y - box.h / 2);
  }

public:
  SDL_FRect box;
  bool debug = true;
  SDL_Color debugColor{255, 0, 0, 255};
};