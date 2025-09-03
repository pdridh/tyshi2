#pragma once

#include "Component.h"
#include "../../../utils/Vec2.h"

struct Transform : public Component
{
public:
  Transform(Entity *parent, Vec2f pos = {0.0, 0.0}, f32 rotation = 0.0, Vec2f scale = {1.0, 1.0})
      : Component{parent}, position{pos}
  {
  }

public:
  Vec2f position;
  f32 rotation;
  Vec2f scale;
};