#pragma once

#include "Component.h"
struct Controller : public Component
{
  bool moveUp = false;
  bool moveDown = false;
  bool moveLeft = false;
  bool moveRight = false;
  bool attack = false;

  Controller(Entity *owner) : Component(owner) {}
};
