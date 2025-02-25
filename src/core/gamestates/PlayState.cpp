#include "PlayState.h"
#include "MenuState.h"

PlayState PlayState::m_self;

PlayState *PlayState::instance()
{
  return &m_self;
}

void PlayState::onEnter(Engine &engine)
{
  printf("Entering play...\n");
  world = new World(engine);
}

void PlayState::onExit(Engine &engine)
{
  printf("Exiting play...\n");
  delete world;
  world = nullptr;
}

void PlayState::pause(Engine &engine)
{
}

void PlayState::resume(Engine &engine)
{
}

void PlayState::update(Engine &engine)
{
  if (engine.input.isKeyDownFrame(SDL_SCANCODE_ESCAPE))
  {
    engine.changeState(MenuState::instance());
  }

  i32 scroll = engine.input.mouseScroll();
  if (scroll < 0)
  {
    engine.camera->zoomBy(0.75);
  }
  else if (scroll > 0)
  {
    engine.camera->zoomBy(1.25);
  }

  if (world)
  {
    world->update();
  }
}

void PlayState::draw(Engine &engine)
{
  if (world)
  {
    world->draw();
  }
}
