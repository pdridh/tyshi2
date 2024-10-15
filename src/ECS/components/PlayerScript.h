#pragma once

#include "../../utils/Vec2.h"
#include "../Entity.h"
#include "Component.h"
#include "Transform.h"
#include "Movement.h"
#include "Animator.h"

#include "../../core/Input.h"
#include "../EntityManager.h"

class PlayerScript : public Component
{

public:
  PlayerScript(Entity *parent, Input &input)
      : Component{parent}, m_input{input}
  {
  }

  void update(const double dt)
  {

    Animator &anim = m_parent->getComponent<Animator>();
    Transform &trans = m_parent->getComponent<Transform>();
    Movement &movement = m_parent->getComponent<Movement>();

    // X axis movement
    if (m_input.isKeyPressed(SDL_SCANCODE_A) == m_input.isKeyPressed(SDL_SCANCODE_D))
    {
      movement.velocity.x = 0;
    }
    else if (m_input.isKeyPressed(SDL_SCANCODE_A))
    {
      movement.velocity.x = -1;
    }
    else if (m_input.isKeyPressed(SDL_SCANCODE_D))
    {
      movement.velocity.x = 1;
    }

    // Y axis movement
    if (m_input.isKeyPressed(SDL_SCANCODE_W) == m_input.isKeyPressed(SDL_SCANCODE_S))
    {
      movement.velocity.y = 0;
    }
    else if (m_input.isKeyPressed(SDL_SCANCODE_W))
    {
      movement.velocity.y = -1;
    }
    else if (m_input.isKeyPressed(SDL_SCANCODE_S))
    {
      movement.velocity.y = 1;
    }

    if (m_input.isKeyPressed(SDL_SCANCODE_SPACE))
    {
      tryFish = true;
    }

    if (tryFish)
    {
      // TODO
      // get the tile that the player is facing somehow
      // Check if the tile is fishable tile (rocks are not fishable, water is)
      // If the tile is rock or whatever else
      //    Send a message to the ui
      // If the tile is fishable
      //    Change state to fishing
      //    lock down other inputs
      //    change ui to fishing
      //    process fishing maybe here maybe somewhere else no clue

      movement.velocity = Vec2::ZERO();
      // update attacking and ignore everything else
      attackCounter += dt;
      if (attackCounter >= attackTimer)
      {
        // m_parent->m_manager.addEntity();
        tryFish = false;
        attackCounter = 0.0;
      }
      anim.playAnimation(2, movement.facingDir == Facing::RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
      return;
    }

    if (movement.isIdle())
    {
      anim.playAnimation(0, movement.facingDir == Facing::RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
    }
    else
    {
      anim.playAnimation(1, movement.facingDir == Facing::RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
    }
  }

private:
  Input &m_input;
  bool tryFish = false;
  double attackTimer = 0.6;
  double attackCounter = 0.0;
};