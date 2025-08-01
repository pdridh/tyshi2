#pragma once

#include "../utils/WorldPosition.h"
#include "entitystates/PlayerStates.h"
#include "entitystates/EntityStateMachine.h"
#include <SDL3/SDL.h>

class World;

class Player
{
private:
  EntityStateMachine<Player> *m_stateMachine;

  // TODO Looks like the drawing stuff are related so maybe a Sprite struct?
  i32 m_drawSize = 24;
  size_t m_textureID; // TODO idk how itll shape out but needs a texture fs
public:
  World &world;
  WorldPosition position;
  Vec2f velocity;
  f32 movementSpeed;

public:
  Player(World &world, WorldPosition pos, size_t textureID);
  ~Player();

  void update();
  void draw();
};
