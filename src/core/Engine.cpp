#include "Engine.h"

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
  double lastTime = SDL_GetTicks() / 1000.0;

  // main loop
  while (m_running)
  {
    double newTime = SDL_GetTicks() / 1000.0;
    double elapsedTime = newTime - lastTime;
    lastTime = newTime;
    processInput();
    update(elapsedTime);
    m_runtime += elapsedTime;
    render();
  }
}

bool Engine::init(const std::string &title)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("Engine::init - Failed to init SDL: %s\n", SDL_GetError());
    return false;
  }

  if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
  {
    printf("Engine::init - Failed to initialize IMG_Init: %s\n", IMG_GetError());
    return false;
  }

  // TODO change how the window size works
  m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, m_windowFlags);

  if (!m_window)
  {
    printf("Engine::init - Failed to create window: %s\n", SDL_GetError());
    return false;
  }

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

  input.update();
}

void Engine::update(const double dt)
{
  // Update here
  // reset manager
  manager.refresh();

  manager.update(dt);

  for (int i = 0; i < manager.m_entities.size(); ++i)
  {
  }
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
  manager.draw(m_renderer);
}
