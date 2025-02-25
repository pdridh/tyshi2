#pragma once

#include "../Engine.h"

// Base class for all game states
class GameState
{
protected:
  GameState() {}

public:
  virtual ~GameState() {}
  virtual void onEnter(Engine &engine) = 0;
  virtual void onExit(Engine &engine) = 0;

  virtual void pause(Engine &engine) = 0;
  virtual void resume(Engine &engine) = 0;

  virtual void update(Engine &engine) = 0;
  virtual void draw(Engine &engine) = 0;

  void changeState(Engine &engine,
                   GameState *state)
  {
    engine.changeState(state);
  }
};