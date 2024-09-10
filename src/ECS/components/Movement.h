#pragma once

#include "../../utils/Vec2.h"
#include "../Entity.h"
#include "Component.h"
#include "Transform.h"

enum Facing
{
  RIGHT,
  LEFT,
  MAX_DIRECTIONS
};

class Movement : public Component
{
public:
  Movement(Entity *parent)
      : Component{parent}
  {
  }

  void update(const double dt) override final
  {
    Transform &transform = m_parent->getComponent<Transform>();

    velocity.normalize();
    transform.position.x += walkSpeed.x * velocity.x * dt;
    transform.position.y += walkSpeed.y * velocity.y * dt;

    // Update dir
    if (velocity.x > 0)
    {
      facingDir = Facing::RIGHT;
    }
    else if (velocity.x < 0)
    {
      facingDir = Facing::LEFT;
    }
  }

  void draw(SDL_Renderer *renderer) override final
  {
  }

public:
  Vec2 velocity = {0.0, 0.0};
  Facing facingDir = Facing::RIGHT;
  Vec2 walkSpeed = {0.0, 0.0};
};