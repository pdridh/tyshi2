#pragma once

#include "GameState.h"

class MenuState : public GameState
{
private:
  static MenuState m_self;
  MenuState() {}

public:
  static MenuState *instance();

  void onEnter(Engine &engine) override;
  void onExit(Engine &engine) override;

  void pause(Engine &engine) override;
  void resume(Engine &engine) override;

  void update(Engine &engine) override;
  void draw(Engine &engine) override;
};