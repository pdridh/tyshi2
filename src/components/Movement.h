#pragma once

#include "../utils/Vec2.h"

enum Facing
{
  RIGHT,
  LEFT,
  MAX_DIRECTIONS
};

struct Movement
{
  Vec2 velocity = {0.0, 0.0};
  Facing facingDir = Facing::RIGHT;
  Vec2 walkSpeed = {0.0, 0.0};
};
