#include "PlayState.h"
#include "MenuState.h"

PlayState PlayState::m_self;

PlayState *PlayState::instance()
{
  return &m_self;
}

void PlayState::onEnter(Engine *game)
{
  printf("Entering play...\n");
  world = new World(game);
}

void PlayState::onExit()
{
  printf("Exiting play...\n");
  delete world;
  world = nullptr;
}

void PlayState::pause()
{
}

void PlayState::resume()
{
}

void PlayState::update(Engine *game)
{
  if (game->input.isKeyDownFrame(SDL_SCANCODE_ESCAPE))
  {
    game->changeState(MenuState::instance());
  }

  if (world)
  {
    world->update();
  }
}

void PlayState::draw(Engine *game)
{
  if (world)
  {
    world->draw();
  }
}
