#pragma once

#include <SDL2/SDL.h>
#include "../utils/Vec2.h"
#include <unordered_map>

typedef Uint8 KeyboardButton;
typedef Uint8 MouseButton;

/// @brief States that a mouse button can have
struct MouseButtonState
{
  bool held = false;
  bool clicked = false;
  bool released = false;
};

/// @brief Handles input stuff, thinking of decoupling the sdl stuff from actions (idk)
class Input
{
public:
  Input();

  /***KEYBOARD STUFF****/

  /// @brief Retrieves the current snapshot of the keyboard from SDL's api and updates the m_keyboardState
  void updateKeyState();

  /// @param key SDL_Scancode that SDL api uses
  /// @return true or false based on the m_keyboardState
  bool isKeyPressed(SDL_Scancode key) const;

  /***MOUSE STUFF****/

  /**
   * @brief   Reset the "clicked" and "released" flag for all mouse buttons after the frame the action was done in
   */
  void resetMouse();

  /**
   * @brief   Process the SDL_MouseMotionEvent and retrieves the current position of the cursor in the window
   * @param   e The reference to the event that SDL loads the information into
   */
  void handleMouseMotion(const SDL_MouseMotionEvent &e);

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

  /// @return The current mouse position captured by the handleMouseMotion()
  const Vec2 &getMouseWindowPosition() const;

private:
  std::unordered_map<MouseButton, MouseButtonState> m_buttonMap;
  Vec2 m_mouseWindowPosition;
  const KeyboardButton *m_keyboardState;
};
