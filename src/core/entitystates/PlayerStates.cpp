#include "PlayerStates.h"
#include "../Player.h"
#include "../World.h"
#include "../../utils/Vec2.h"

PlayerIdle *PlayerIdle::instance()
{
  static PlayerIdle instance;

  return &instance;
}

void PlayerIdle::onEnter(Player &player)
{
  printf("Enting player idle\n");
}

void PlayerIdle::execute(Player &player)
{
  Vec2f dir = Vec2f::ZERO();
  Input &input = player.world.m_engine.input;

  Vec2f oldPosition = player.position.actualPosition;

  if (input.isKeyPressed(SDL_SCANCODE_W))
  {
    dir += Vec2f(0, 1);
  }
  else if (input.isKeyPressed(SDL_SCANCODE_S))
  {
    dir += Vec2f(0, -1);
  }

  if (input.isKeyPressed(SDL_SCANCODE_A))
  {
    dir += Vec2f(-1, 0);
  }
  else if (input.isKeyPressed(SDL_SCANCODE_D))
  {
    dir += Vec2f(1, 0);
  }
  Vec2f move = dir.normalize().scale(player.movementSpeed).scale(player.world.m_engine.dt);
  Vec2f newPosition = oldPosition + move;

  // TODO restrain player somehow idk
  if (newPosition.x < 0)
  {
    newPosition.x = 0;
  }

  player.position = player.world.actualToWorld(newPosition);
  player.world.m_engine.camera->centerOn(player.position.actualPosition);
}

void PlayerIdle::onExit(Player &player)
{
  printf("Exiting idle state\n");
}
