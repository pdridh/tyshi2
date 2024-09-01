#include "Engine.h"

const double Engine::FIXED_TIMESTEP = 1.0 / 60.0;
const SDL_Color Engine::RENDER_CLEAR_COLOR = {0, 0, 0, 255}; // Black clear color
const int Engine::SCREEN_WIDTH = 800;
const int Engine::SCREEN_HEIGHT = 600;

Engine::Engine(const std::string &title)
{
  m_running = true;
  if (!init(title))
  {
    printf("Engine::Engine - Failed to initialize\n");
    m_running = false;
  }

  // All time related stuff in seconds
  double accumulatedTime = 0.0;
  double lastTime = SDL_GetTicks() / 1000.0;

  // main loop
  while (m_running)
  {
    double newTime = SDL_GetTicks() / 1000.0;
    double elapsedTime = newTime - lastTime;
    lastTime = newTime;

    accumulatedTime += elapsedTime;

    processInput();
    while (accumulatedTime > FIXED_TIMESTEP)
    {
      processInput();
      update(FIXED_TIMESTEP);
      accumulatedTime -= FIXED_TIMESTEP;
    }
    render();
  }
}

bool Engine::init(const std::string &title)
{
  // TODO change how the window size works
  m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, m_windowFlags);

  if (!m_window)
  {
    printf("Engine::init - Failed to create window: %s\n", SDL_GetError());
    return false;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  m_renderer = SDL_CreateRenderer(m_window, -1, m_rendererFlags);

  if (!m_renderer)
  {
    printf("Engine::init - Failed to create renderer: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void Engine::processInput()
{
  // Process events
  SDL_Event event;

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      m_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        m_running = false;
      }
    default:
      break;
    }
  }
}

void Engine::update(const double dt)
{
  // Update here
}

void Engine::render()
{
  // Clear screen
  SDL_SetRenderDrawColor(m_renderer, RENDER_CLEAR_COLOR.r, RENDER_CLEAR_COLOR.g, RENDER_CLEAR_COLOR.b, RENDER_CLEAR_COLOR.a);
  SDL_RenderClear(m_renderer);
  // Draw stuff
  renderGame();
  // Present screen
  SDL_RenderPresent(m_renderer);
}

void Engine::renderGame()
{
  // Draw stuff here
}
