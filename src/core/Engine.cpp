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
      m_runtime += FIXED_TIMESTEP;
    }
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

  SDL_Texture *text = IMG_LoadTexture(m_renderer, "assets/images/knight.png");
  SDL_Texture *enemText = IMG_LoadTexture(m_renderer, "assets/images/goblin.png");

  Entity &e = manager.addEntity();

  Transform &trans = e.getComponent<Transform>();
  trans.position = Vec2(200, 300);

  Animator &anim = e.addComponent<Animator>(text, 10);
  anim.addAnimation(0, 0, 0, 6, 192, 192, 10);
  anim.addAnimation(1, 0, 1, 6, 192, 192, 10);
  anim.addAnimation(2, 0, 3, 6, 192, 192, 10);

  Movement &move = e.addComponent<Movement>();
  move.walkSpeed = Vec2(100, 100);

  e.addComponent<Collider>(50, 50);

  e.addComponent<PlayerScript>(input);

  Entity &enem = manager.addEntity();

  Transform &etrans = enem.getComponent<Transform>();
  etrans.position = Vec2(400, 100);

  Animator &eanim = enem.addComponent<Animator>(enemText, 10);
  eanim.addAnimation(0, 0, 0, 7, 192, 192, 10);
  eanim.addAnimation(1, 0, 1, 6, 192, 192, 10);
  eanim.addAnimation(2, 0, 2, 6, 192, 192, 10);

  Movement &emove = enem.addComponent<Movement>();
  emove.walkSpeed = Vec2(50, 50);
  Collider &ecollide = enem.addComponent<Collider>(100, 100);
  // anim.addAnimation(1, 0, 1, 6, 192, 192, 10);
  // anim.addAnimation(2, 0, 3, 6, 192, 192, 10);

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
  input.updateKeyState();
}

void Engine::update(const double dt)
{
  // Update here
  // reset manager
  manager.refresh();

  manager.update(dt);

  for (int i = 0; i < manager.m_entities.size(); ++i)
  {
    if (!manager.m_entities[i]->hasComponent<Collider>())
      continue;

    // Here means it has the component so check others
    for (int j = i + 1; j < manager.m_entities.size(); ++j)
    {
      Collider &iCollider = manager.m_entities[i]->getComponent<Collider>();
      Collider &jCollider = manager.m_entities[j]->getComponent<Collider>();

      if (iCollider.box.x <= jCollider.box.x + jCollider.box.w &&
          iCollider.box.x + iCollider.box.w >= jCollider.box.x &&
          iCollider.box.y <= jCollider.box.y + jCollider.box.h &&
          iCollider.box.y + iCollider.box.h >= jCollider.box.y)
      {
        // iCollider.triggerCollision();
        jCollider.triggerCollision();
      }
    }
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
