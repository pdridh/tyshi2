#include "EditorState.hpp"
#include "MenuState.hpp"
#include "PlayState.hpp"

EditorState EditorState::m_self;

EditorState *EditorState::instance()
{
  return &m_self;
}

void EditorState::onEnter(Engine *game)
{
  printf("Entering editor...\n");

  game->camera->centerOn(Vec2f(800 / 2, 600 / 2));

  // SDL_Texture *text = IMG_LoadTexture(game->m_renderer, "assets/tileset.png");
  world = new World(game);
}

void EditorState::onExit()
{
  printf("Exiting editor...\n");
}

void EditorState::pause()
{
}

void EditorState::resume()
{
}

void EditorState::update(Engine *game)
{
  if (game->input.isKeyDownFrame(SDL_SCANCODE_ESCAPE))
  {
    game->changeState(MenuState::instance());
  }
  else if (game->input.isKeyDownFrame(SDL_SCANCODE_SPACE))
  {
    game->changeState(PlayState::instance());
  }

  int speed = 10;
  // Camera updater
  if (game->input.isKeyDownFrame(SDL_SCANCODE_A))
  {
    currentPos += Vec2f(-speed, 0);
  }
  if (game->input.isKeyDownFrame(SDL_SCANCODE_D))
  {
    currentPos += Vec2f(speed, 0);
  }
  if (game->input.isKeyDownFrame(SDL_SCANCODE_W))
  {
    currentPos += Vec2f(0, -speed);
  }
  if (game->input.isKeyDownFrame(SDL_SCANCODE_S))
  {
    currentPos += Vec2f(0, speed);
  }

  if (game->input.mouseScroll() != 0)
  {
    currentCode += game->input.mouseScroll();
  }

  if (game->input.isMouseClicked(SDL_BUTTON_LEFT))
  {
    // Lets say current tile held has atlas code 2

    Vec2f mousePos = game->input.getMouseClickPosition(SDL_BUTTON_LEFT);
    Vec2f mouseWorldPos = game->camera->screenToWorld(mousePos);
    world->changeChunkTile(mouseWorldPos, currentCode);
  }

  game->camera->centerOn(currentPos);
}

void EditorState::draw(Engine *game)
{
  world->draw();
}