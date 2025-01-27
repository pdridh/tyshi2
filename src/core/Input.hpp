#pragma once

#include <SDL2/SDL.h>

#include "../utils/Vec2.hpp"

#include <unordered_map>
#include <vector>

typedef u_int8_t MouseButton;

/// @brief States that a mouse button can have
struct MouseButtonState
{
  bool held = false;
  bool clicked = false;
  bool released = false;

  // For position at this state
  Vec2f position;
};

/// @brief Handles input stuff, thinking of decoupling the sdl stuff from actions (idk)
class Input
{
private:
  std::unordered_map<MouseButton, MouseButtonState> m_buttonMap;
  std::vector<bool> m_keysDownThisFrame;
  std::vector<bool> m_keysUpThisFrame;
  int mouseWheelY;

private:
  // Reset keysdown and keysup this frame
  void resetKeyboard();

  // Reset the MouseButtonState for all butons
  void resetMouse();

public:
  Input();

  // Reset both keuyboard and mouse
  void reset();

  /***KEYBOARD STUFF****/

  void handleKeyButtonDown(SDL_KeyboardEvent &e);
  void handleKeyButtonUp(SDL_KeyboardEvent &e);

  // Checks if a key was pressed down THIS frame (resets every frame unless pressed down)
  bool isKeyDownFrame(SDL_Scancode key) const;

  // Checks if a key was let go THIS frame (resets every frame)
  bool isKeyUpFrame(SDL_Scancode key) const;

  /***MOUSE STUFF****/

  /**
   * @brief   Process the SDL_MouseMotionEvent and retrieves the current position of the cursor in the window
   * @param   e The reference to the event that SDL loads the information into
   */
  void handleMouseMotion(const SDL_MouseMotionEvent &e);
  void handleMouseWheel(const SDL_MouseWheelEvent &e);

  /**
   * @brief   Process the SDL_MouseButtonEvent and update the mousestate based on the mouse button clicked
   *          Expect the caller to send a MOUSEDOWN event and doesnt really care
   * @param   e The reference to the event that contains all the information like the button pressed: e.button
   */
  void handleMouseButtonDown(const SDL_MouseButtonEvent &e);

  /**
   * @brief   Process the SDL_MouseButtonEvent and update the mousestate based on the mouse button clicked
   *          Expect the caller to send a MOUSEUP event and doesnt really care
   * @param   e The reference to the event that contains all the information like the button pressed: e.button
   */
  void handleMouseButtonUp(const SDL_MouseButtonEvent &e);

  /// @brief Check if the mouse button is clicked for the current frame
  /// @param btn The btn index that SDL uses SDL_Button
  /// @return boolean based on the state
  bool isMouseClicked(MouseButton btn) const;

  /// @brief Check if the mouse button is held right now
  /// @param btn The btn index that SDL uses SDL_Button
  /// @return boolean based on the state
  bool isMouseHeld(MouseButton btn) const;

  /// @brief Check if the mouse button got released for the current frame
  /// @param btn The btn index that SDL uses SDL_Button
  /// @return boolean based on the state
  bool isMouseReleased(MouseButton btn) const;

  int mouseScroll() const { return mouseWheelY; }

  /// @return The position that this btn was clicked on
  const Vec2f getMouseClickPosition(MouseButton btn) const
  {
    const MouseButtonState &state = m_buttonMap.at(btn);
    return state.position;
  }
};
