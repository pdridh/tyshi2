#include "Input.h"

Input::Input()
{
  m_keyboardState = {};
  m_buttonMap[SDL_BUTTON_LEFT] = {};
  m_buttonMap[SDL_BUTTON_RIGHT] = {};
  m_buttonMap[SDL_BUTTON_MIDDLE] = {};
}

bool Input::isKeyPressed(SDL_Scancode key) const
{
  return m_keyboardState[key];
}

void Input::updateKeyState()
{
  m_keyboardState = SDL_GetKeyboardState(NULL);
}

void Input::resetMouse()
{
  for (auto &btn : m_buttonMap)
  {
    btn.second.clicked = false;
    btn.second.released = false;
  }
}

void Input::handleMouseButtonDown(const SDL_MouseButtonEvent &e)
{
  MouseButtonState &currentState = m_buttonMap[e.button];
  if (!currentState.held)
  {
    currentState.clicked = true;
  }
  currentState.held = true;
  currentState.position = Vec2(e.x, e.y);
}

void Input::handleMouseButtonUp(const SDL_MouseButtonEvent &e)
{
  MouseButtonState &currentState = m_buttonMap[e.button];
  currentState.released = true;
  currentState.held = false;
  currentState.position = Vec2(e.x, e.y);
}

bool Input::isMouseClicked(MouseButton btn) const
{
  return m_buttonMap.at(btn).clicked;
}

bool Input::isMouseHeld(MouseButton btn) const
{
  return m_buttonMap.at(btn).held;
}

bool Input::isMouseReleased(MouseButton btn) const
{
  return m_buttonMap.at(btn).released;
}
