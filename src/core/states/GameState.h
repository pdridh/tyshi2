#pragma once

#include "../Engine.h"

// Base class for all game states
class GameState
{
protected:
  GameState() {}

public:
  virtual void onEnter(Engine *game) = 0;
  virtual void onExit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;

  virtual void update(Engine *game) = 0;
  virtual void draw(Engine *game) = 0;

  void changeState(Engine *game,
                   GameState *state)
  {
    game->changeState(state);
  }
};