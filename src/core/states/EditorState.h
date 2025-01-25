#pragma once

#include "GameState.h"
#include "../World.h"

class EditorState : public GameState
{
private:
  static EditorState m_self;
  EditorState() {}

  Vec2f currentPos = Vec2f();
  World *world;
  int currentCode = 1;

public:
  static EditorState *instance();

  void onEnter(Engine *game) override;
  void onExit() override;

  void pause() override;
  void resume() override;

  void update(Engine *game) override;
  void draw(Engine *game) override;
};