#include "Input.h"
#include <algorithm>

Input::Input()
{
  m_keysDownThisFrame.resize(SDL_NUM_SCANCODES, false);
  m_keysUpThisFrame.resize(SDL_NUM_SCANCODES, false);
  m_buttonMap[SDL_BUTTON_LEFT] = {};
  m_buttonMap[SDL_BUTTON_RIGHT] = {};
  m_buttonMap[SDL_BUTTON_MIDDLE] = {};
}

void Input::reset()
{
  resetKeyboard();
  resetMouse();
}

void Input::resetKeyboard()
{
  std::fill(m_keysDownThisFrame.begin(), m_keysDownThisFrame.end(), false);
  std::fill(m_keysUpThisFrame.begin(), m_keysUpThisFrame.end(), false);
}

void Input::handleKeyButtonDown(SDL_KeyboardEvent &e)
{
  m_keysDownThisFrame[e.keysym.scancode] = true;
}

void Input::handleKeyButtonUp(SDL_KeyboardEvent &e)
{
  m_keysUpThisFrame[e.keysym.scancode] = true;
}

bool Input::isKeyDownFrame(SDL_Scancode key) const
{
  return m_keysDownThisFrame[key];
}

bool Input::isKeyUpFrame(SDL_Scancode key) const
{
  return m_keysUpThisFrame[key];
}

void Input::resetMouse()
{
  for (auto &btn : m_buttonMap)
  {
    btn.second.clicked = false;
    btn.second.released = false;
  }
  mouseWheelY = 0;
}

void Input::handleMouseWheel(const SDL_MouseWheelEvent &e)
{
  mouseWheelY = e.y;
}

void Input::handleMouseButtonDown(const SDL_MouseButtonEvent &e)
{
  MouseButtonState &currentState = m_buttonMap[e.button];
  if (!currentState.held)
  {
    currentState.clicked = true;
  }
  currentState.held = true;
  currentState.position = Vec2f(e.x, e.y);
}

void Input::handleMouseButtonUp(const SDL_MouseButtonEvent &e)
{
  MouseButtonState &currentState = m_buttonMap[e.button];
  currentState.released = true;
  currentState.held = false;
  currentState.position = Vec2f(e.x, e.y);
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
