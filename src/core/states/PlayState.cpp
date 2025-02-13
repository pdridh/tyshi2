#include "PlayState.hpp"
#include "MenuState.hpp"

PlayState PlayState::m_self;

PlayState *PlayState::instance()
{
  return &m_self;
}

void PlayState::onEnter(Engine *game)
{
  world = new World(game);
  printf("Entering play...\n");
}

void PlayState::onExit()
{
  delete world;
  printf("Exiting play...\n");
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
}

void PlayState::draw(Engine *game)
{
}
