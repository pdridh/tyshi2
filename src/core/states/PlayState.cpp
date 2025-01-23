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
}

void PlayState::onExit()
{
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
  if (game->input.isKeyPressed(SDL_SCANCODE_M))
  {
    game->changeState(MenuState::instance());
  }
}

void PlayState::draw(Engine *game)
{
}
