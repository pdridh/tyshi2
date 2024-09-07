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

  ecs.registerComponent<Sprite>();
  ecs.registerComponent<Movement>();
  ecs.registerComponent<Transform>();
  ecs.registerComponent<Controller>();
  ecs.registerComponent<Animator>();

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
  input.updateKeyState();

  controllerSystem(dt);
  movementSystem(dt);
  movementAnimationSystem(dt);
  animationSystem(dt);

  // Update here
}

void Engine::controllerSystem(double dt)
{

  for (auto &[id, movement, controller] : ecs.view<Movement, Controller>())
  {
    // X axis movement
    if (input.isKeyPressed(SDL_SCANCODE_A) == input.isKeyPressed(SDL_SCANCODE_D))
    {
      movement.velocity.x = 0;
    }
    else if (input.isKeyPressed(SDL_SCANCODE_A))
    {
      movement.velocity.x = -1;
    }
    else if (input.isKeyPressed(SDL_SCANCODE_D))
    {
      movement.velocity.x = 1;
    }

    // Y axis movement
    if (input.isKeyPressed(SDL_SCANCODE_W) == input.isKeyPressed(SDL_SCANCODE_S))
    {
      movement.velocity.y = 0;
    }
    else if (input.isKeyPressed(SDL_SCANCODE_W))
    {
      movement.velocity.y = -1;
    }
    else if (input.isKeyPressed(SDL_SCANCODE_S))
    {
      movement.velocity.y = 1;
    }
  }
}

void Engine::movementSystem(double dt)
{
  for (auto &[id, transform, movement] : ecs.view<Transform, Movement>())
  {
    movement.velocity.normalize();
    transform.position.x += movement.walkSpeed.x * movement.velocity.x * dt;
    transform.position.y += movement.walkSpeed.y * movement.velocity.y * dt;

    // Update dir
    if (movement.velocity.x > 0)
    {
      movement.facingDir = Facing::RIGHT;
    }
    else if (movement.velocity.x < 0)
    {
      movement.facingDir = Facing::LEFT;
    }
  }
}

void Engine::movementAnimationSystem(double dt)
{
  for (auto &[id, movement, animator] : ecs.view<Movement, Animator>())
  {
    if (movement.velocity.x || movement.velocity.y)
    {
      switch (movement.facingDir)
      {
      case Facing::RIGHT:
        // animator.
        animator.playAnimation(1);
        break;
      case Facing::LEFT:
      default:
        animator.playAnimation(1, SDL_FLIP_HORIZONTAL);
        break;
      }
    }
    else // ur not moving
    {
      switch (movement.facingDir)
      {
      case Facing::RIGHT:
        animator.playAnimation(0);
        break;
      case Facing::LEFT:
      default:
        animator.playAnimation(0, SDL_FLIP_HORIZONTAL);
        break;
      }
    }
  }
}

void Engine::animationSystem(double dt)
{
  // Change animations here
  // + calculate animation stuff
  for (auto &[id, animator] : ecs.view<Animator>())
  {
    if (!animator.animationsAdded)
    {
      continue;
    }

    Animation *currentAnim = animator.animations[animator.currentAnimation];
    currentAnim->frameTime += dt;
    // Update this animation
    if (currentAnim->frameTime >= currentAnim->frameDuration)
    {
      currentAnim->frameTime = 0;
      currentAnim->currentFrame++;
      currentAnim->currentFrame = currentAnim->currentFrame % currentAnim->frames;
      currentAnim->frameRect.x = (currentAnim->startX * currentAnim->frameRect.w) +
                                 currentAnim->frameRect.w * currentAnim->currentFrame;
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
  renderSpriteSystem();
  renderAnimationSystem();
}

void Engine::renderSpriteSystem()
{
  // Requires Sprite and Transform
  for (auto &[id, sprite, transform] : ecs.view<Sprite, Transform>())
  {
    // Get the center for rotations
    SDL_FPoint center;
    center.x = (float)transform.position.x;
    center.y = (float)transform.position.y;

    sprite.destinationRect.w = sprite.textureRect.w * sprite.scale.x;
    sprite.destinationRect.h = sprite.textureRect.h * sprite.scale.y;
    // Position is center so box should be started from top-left (half of width)
    sprite.destinationRect.x = transform.position.x - (sprite.destinationRect.w / 2.f);
    sprite.destinationRect.y = transform.position.y - (sprite.destinationRect.h / 2.f);

    if (sprite.debug)
    {
      SDL_SetRenderDrawColor(m_renderer, sprite.debugBoxColor.r, sprite.debugBoxColor.g, sprite.debugBoxColor.b, sprite.debugBoxColor.a);
      SDL_RenderDrawRectF(m_renderer, &sprite.destinationRect);
    }

    SDL_RenderCopyExF(m_renderer, sprite.texture, &sprite.textureRect, &sprite.destinationRect, sprite.rotateAngle, &center, sprite.flip);
  }
}

void Engine::renderAnimationSystem()
{
  for (auto &[id, animator, transform] : ecs.view<Animator, Transform>())
  {
    if (!animator.animationsAdded)
    {
      continue;
    }

    Animation *currentAnim = animator.animations[animator.currentAnimation];

    SDL_FRect dstRect;
    dstRect.w = currentAnim->frameRect.w * animator.scale.x,
    dstRect.h = currentAnim->frameRect.h * animator.scale.y;
    // Position is center so box should be started from top-left (half of width)
    dstRect.x = transform.position.x - (dstRect.w / 2.f);
    dstRect.y = transform.position.y - (dstRect.h / 2.f);
    SDL_FPoint center = {(float)transform.position.x, (float)transform.position.y};

    SDL_RenderCopyExF(m_renderer, animator.sheet,
                      &currentAnim->frameRect, &dstRect,
                      currentAnim->angle, &center, animator.renderFlipped);
  }
}
