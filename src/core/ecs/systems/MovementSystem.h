#pragma once

#include "../EntityManager.h"
#include "../components/Transform.h"
#include "../components/Controller.h"

void MovementSystem(EntityManager *entityManager, f32 dt)
{
  for (auto &e : entityManager->m_entities)
  {
    if (!e->hasComponent<Controller>() || !e->hasComponent<Transform>())
    {
      continue;
    }

    // TODO make an actual movement/kinematic/physicbody component for ts

    Transform &transform = e->getComponent<Transform>();
    Controller &controller = e->getComponent<Controller>();

    if (controller.moveUp)
    {
      transform.position.y += 100 * dt;
    }
    else if (controller.moveDown)
    {
      transform.position.y -= 100 * dt;
    }

    if (controller.moveLeft)
    {
      transform.position.x -= 100 * dt;
    }
    else if (controller.moveRight)
    {
      transform.position.x += 100 * dt;
    }
  }
}