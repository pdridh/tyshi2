#pragma once

#include "GameState.hpp"
#include "../World.hpp"

class PlayState : public GameState
{
private:
  static PlayState m_self;
  PlayState() {}
  World *world;

public:
  static PlayState *instance();

  void onEnter(Engine *game) override;
  void onExit() override;

  void pause() override;
  void resume() override;

  void update(Engine *game) override;
  void draw(Engine *game) override;
};