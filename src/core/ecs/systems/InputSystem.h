#pragma once

#include "../components/Controller.h"
#include "../EntityManager.h"
#include "../../Input.h"

void InputSystem(EntityManager *manager, Input *input)
{
  for (auto &e : manager->m_entities)
  {
    if (!e->hasComponent<Controller>())
    {
      continue;
    }

    Controller &controller = e->getComponent<Controller>();
    // TODO change this to actions
    controller.moveUp = input->isKeyPressed(SDL_SCANCODE_W);
    controller.moveDown = input->isKeyPressed(SDL_SCANCODE_S);
    controller.moveLeft = input->isKeyPressed(SDL_SCANCODE_A);
    controller.moveRight = input->isKeyPressed(SDL_SCANCODE_D);
  }
}