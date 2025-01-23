#include "MenuState.h"
#include "PlayState.h"

MenuState MenuState::m_self;

MenuState *MenuState::instance()
{
  return &m_self;
}

void MenuState::onEnter(Engine *game)
{

  printf("Entering menu...\n");
}

void MenuState::onExit()
{
  printf("Exiting menu...\n");
}

void MenuState::pause()
{
}

void MenuState::resume()
{
}

void MenuState::update(Engine *game)
{
  if (game->input.isKeyDownFrame(SDL_SCANCODE_ESCAPE))
  {
    game->quit();
  }
  else if (game->input.isKeyDownFrame(SDL_SCANCODE_SPACE))
  {
    game->changeState(PlayState::instance());
  }
}

void MenuState::draw(Engine *game)
{
}