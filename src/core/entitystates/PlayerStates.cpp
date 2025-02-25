#include "PlayerStates.h"

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
  printf("Player is idle\n");
}

void PlayerIdle::onExit(Player &player)
{
  printf("Exiting idle state\n");
}
