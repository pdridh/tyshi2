#pragma once

#include <SDL2/SDL.h>
#include "../utils/Vec2.h"
#include <unordered_map>

typedef Uint8 KeyboardButton;
typedef Uint8 MouseButton;

// #typedef MouseButton Uint8

struct MouseButtonState
{
  bool held = false;
  bool clicked = false;
  bool released = false;
};

class Input
{
public:
  Input()
  {
    m_keyboardState = {};
    buttonMap[SDL_BUTTON_LEFT] = {};
    buttonMap[SDL_BUTTON_RIGHT] = {};
    buttonMap[SDL_BUTTON_MIDDLE] = {};
  }

  bool isKeyPressed(SDL_Scancode key) const
  {
    return m_keyboardState[key];
  }

  // Retrieves the current snapshot of the keyboard from SDL's api
  void updateKeyState()
  {
    m_keyboardState = SDL_GetKeyboardState(NULL);
  }

  void resetMouse()
  {
    for (auto &btn : buttonMap)
    {
      btn.second.clicked = false;
      btn.second.released = false;
    }
  }

  void handleMouseButtonDown(const SDL_MouseButtonEvent &e)
  {
    MouseButtonState &currentState = buttonMap[e.button];
    if (!currentState.held)
    {
      currentState.clicked = true;
    }
    currentState.held = true;
  }

  void handleMouseButtonUp(const SDL_MouseButtonEvent &e)
  {
    MouseButtonState &currentState = buttonMap[e.button];
    currentState.released = true;
    currentState.held = false;
  }

  bool isMouseClicked(MouseButton btn) const
  {
    return buttonMap.at(btn).clicked;
  }

  bool isMouseHeld(MouseButton btn) const
  {
    return buttonMap.at(btn).held;
  }

  bool isMouseReleased(MouseButton btn) const
  {
    return buttonMap.at(btn).released;
  }

private:
  std::unordered_map<MouseButton, MouseButtonState> buttonMap;
  const KeyboardButton *m_keyboardState;
};
