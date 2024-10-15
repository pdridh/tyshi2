#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

#include "Input.h"

#include "../ECS/components/Transform.h"
#include "../ECS/components/Movement.h"
#include "../ECS/components/Animator.h"
#include "../ECS/components/PlayerScript.h"
#include "../ECS/components/Collider.h"
#include "../ECS/EntityManager.h"

// Main engine class that handles the main loop, window creation and frame speed stuff
// TODO: lots of changes (eg. read config from file and stuff)
class Engine
{
public:
  Engine(const std::string &window_title);

private:
  // Static
  // TODO read these from configs
  static const double FIXED_TIMESTEP;
  static const SDL_Color RENDER_CLEAR_COLOR;
  static const int SCREEN_WIDTH;
  static const int SCREEN_HEIGHT;

  double m_runtime; // Total time the engine ran

  bool m_running;
  // Dt related stuff

  // Window related stuff
  int m_windowFlags; // Maybe upgrade to opengl later and stuff
  SDL_Window *m_window;

  // Renderer stuff
  int m_rendererFlags;
  SDL_Renderer *m_renderer;

  Input input;

  EntityManager manager;
  // Initialize helper function for constructor to init stuff
  bool init(const std::string &window_title);

  // Process and handle input events
  void processInput();

  // Update game
  void update(double dt);

  // Render frame that clears and swaps buffer each frame
  void render();
  // Main render function for rendering the game
  void renderGame();
  // render systems
};