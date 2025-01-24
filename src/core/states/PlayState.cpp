#include "PlayState.h"
#include "MenuState.h"
#include "EditorState.h"

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
  if (game->input.isKeyDownFrame(SDL_SCANCODE_ESCAPE))
  {
    game->changeState(MenuState::instance());
  }
  else if (game->input.isKeyDownFrame(SDL_SCANCODE_GRAVE))
  {
    game->changeState(EditorState::instance());
  }
}

void PlayState::draw(Engine *game)
{
}
