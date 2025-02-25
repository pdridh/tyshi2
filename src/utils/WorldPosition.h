#pragma once

#include "Vec2.h"

struct WorldPosition
{
  Vec2i nthChunk;
  Vec2i nthTile;
  Vec2f actualPosition;
};