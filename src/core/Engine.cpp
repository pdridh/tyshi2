#include "Engine.h"
#include "states/PlayState.h"
#include "states/MenuState.h"

const SDL_Color Engine::RENDER_CLEAR_COLOR = {0, 0, 0, 255}; // Black clear color
const i32 Engine::SCREEN_WIDTH = 800;
const i32 Engine::SCREEN_HEIGHT = 600;

Engine::Engine(const std::string &title)
{
  m_running = true;
  if (!init(title))
  {
    printf("Engine::Engine - Failed to initialize\n");
    m_running = false;
  }

  // All time related stuff in seconds
  f32 lastTime = SDL_GetTicks() / 1000.0;

  // main loop
  while (m_running)
  {
    f32 newTime = SDL_GetTicks() / 1000.0;
    f32 elapsedTime = newTime - lastTime;
    lastTime = newTime;
    processInput();
    dt = elapsedTime;
    update();
    m_runtime += elapsedTime;
    render();
  }
}

bool Engine::init(const std::string &title)
{
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0)
  {
    printf("Engine::init - Failed to init SDL: %s\n", SDL_GetError());
    return false;
  }

  m_windowFlags = SDL_WINDOW_RESIZABLE;
  // TODO change how the window size works
  bool success = SDL_CreateWindowAndRenderer(title.c_str(),
                                             SCREEN_WIDTH, SCREEN_HEIGHT,
                                             m_windowFlags, &m_window, &m_renderer);

  if (!success)
  {
    printf("Engine::init - Failed to create window and renderer: %s\n", SDL_GetError());
    return false;
  }

  const char *version = "2.0.0";
  const char *appid = "com.tyshi.tyshi2";
  SDL_SetAppMetadata(title.c_str(), version, appid);

  camera = new Camera(m_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  changeState(MenuState::instance());

  return true;
}

void Engine::processInput()
{

  // Process events
  SDL_Event event;

  input.reset();

  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_EVENT_QUIT:
      m_running = false;
      break;
    case SDL_EVENT_KEY_DOWN:
      input.handleKeyButtonDown(event.key);
      break;
    case SDL_EVENT_KEY_UP:
      input.handleKeyButtonUp(event.key);
      break;
    case SDL_EVENT_MOUSE_WHEEL:
      // event
      input.handleMouseWheel(event.wheel);
      break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
      input.handleMouseButtonUp(event.button);
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      input.handleMouseButtonDown(event.button);
      break;
    default:
      break;
    }
  }
  input.updateKeyboardState();
}

void Engine::changeState(GameState *state)
{
  // Exit from current state and
  // Remove it from the states stack
  if (!m_states.empty())
  {
    m_states.back()->onExit();
    m_states.pop_back();
  }

  // Change to another state completely
  m_states.push_back(state);
  m_states.back()->onEnter(this);
}

void Engine::addState(GameState *state)
{
  // pause current state
  if (!m_states.empty())
  {
    m_states.back()->pause();
  }

  // Add a new state above this state
  m_states.push_back(state);
  m_states.back()->onEnter(this);
}

void Engine::exitState()
{
  // Pop current state
  if (!m_states.empty())
  {
    m_states.back()->onExit();
    m_states.pop_back();
  }

  // resume previous state
  if (!m_states.empty())
  {
    m_states.back()->resume();
  }
}

void Engine::quit()
{
  clean();
  m_running = false;
}

void Engine::update()
{
  // Update top of the stack
  m_states.back()->update(this);
}

void Engine::renderGame()
{
  m_states.back()->draw(this);
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

void Engine::clean()
{
  // TODO
  // Delete memory stuff
  // Handle everything
  // quit sdl and img shit
}