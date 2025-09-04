#include "MenuState.h"
#include "PlayState.h"

MenuState MenuState::m_self;

MenuState *MenuState::instance()
{
  return &m_self;
}

void MenuState::onEnter(Engine &engine)
{
  printf("Entering menu...\n");
}

void MenuState::onExit(Engine &engine)
{
  printf("Exiting menu...\n");
}

void MenuState::pause(Engine &engine)
{
}

void MenuState::resume(Engine &engine)
{
}

void MenuState::update(Engine &engine)
{
  if (engine.input->isKeyDownFrame(SDL_SCANCODE_ESCAPE))
  {
    engine.quit();
  }
  else if (engine.input->isKeyDownFrame(SDL_SCANCODE_SPACE))
  {
    engine.changeState(PlayState::instance());
  }
}

void MenuState::draw(Engine &engine)
{
}