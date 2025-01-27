#pragma once

#include "GameState.hpp"

class MenuState : public GameState
{
private:
  static MenuState m_self;
  MenuState() {}

public:
  static MenuState *instance();

  void onEnter(Engine *game) override;
  void onExit() override;

  void pause() override;
  void resume() override;

  void update(Engine *game) override;
  void draw(Engine *game) override;
};