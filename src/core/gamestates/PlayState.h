#pragma once

#include "GameState.h"
#include "../World.h"

class PlayState : public GameState
{
private:
  static PlayState m_self;
  PlayState() {}
  World *world;

public:
  static PlayState *instance();

  void onEnter(Engine &engine) override;
  void onExit(Engine &engine) override;

  void pause(Engine &engine) override;
  void resume(Engine &engine) override;

  void update(Engine &engine) override;
  void draw(Engine &engine) override;
};