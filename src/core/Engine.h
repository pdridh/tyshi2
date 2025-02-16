#pragma once

#include "core.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "Input.h"
#include "Camera.h"

class GameState;

// Main engine class that handles the main loop, window creation and frame speed stuff
// TODO: lots of changes (eg. read config from file and stuff)
class Engine
{
public:
  Engine(const std::string &window_title);

private:
  // Static
  // TODO read these from configs
  static const f32 FIXED_TIMESTEP;
  static const SDL_Color RENDER_CLEAR_COLOR;
  static const i32 SCREEN_WIDTH;
  static const i32 SCREEN_HEIGHT;

  f32 m_runtime; // Total time the engine ran

  bool m_running;
  // Dt related stuff

  // Window related stuff
  i32 m_windowFlags; // Maybe upgrade to opengl later and stuff
  SDL_Window *m_window;

  std::vector<GameState *> m_states;

public:
  f32 dt;

  // Renderer stuff
  i32 m_rendererFlags;

  SDL_Renderer *m_renderer;

  Input input;
  Camera *camera;

  // State related stuff
  void changeState(GameState *state);
  void addState(GameState *state);
  void exitState();

  void quit();

private:
  // Initialize helper function for constructor to init stuff
  bool init(const std::string &window_title);

  // Process and handle input events
  void processInput();

  // Update game
  void update();
  // Render frame that clears and swaps buffer each frame
  void render();

  // Main render function for rendering the game
  void renderGame();

  void clean();
};