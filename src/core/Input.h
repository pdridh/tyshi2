#pragma once

#include <SDL2/SDL.h>

class Input
{
public:
  Input()
  {
    m_keyboardState = {};
  }

  void updateKeyState()
  {
    m_keyboardState = SDL_GetKeyboardState(NULL);
  }

  bool isKeyPressed(SDL_Scancode key) const
  {
    return m_keyboardState[key];
  }

private:
  const Uint8 *m_keyboardState;
};
