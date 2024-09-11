#pragma once

#include "Component.h"
#include "../../utils/Vec2.h"

class Transform : public Component
{
public:
  Transform(Entity *parent, Vec2 pos = {0.0, 0.0})
      : Component{parent}, position{pos}
  {
  }

  void update(const double dt) override final
  {
  }

  void draw(SDL_Renderer *renderer) override final
  {
  }

public:
  Vec2 position;
};