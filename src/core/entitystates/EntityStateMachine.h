#pragma once

#include <cassert>
#include <string>

#include "State.h"

template <class EntityType>
class EntityStateMachine
{
private:
  EntityType &m_owner;

public:
  State<EntityType> *currentState;
  State<EntityType> *previousState;
  State<EntityType> *globalState;

public:
  EntityStateMachine(EntityType &owner) : m_owner{owner},
                                          currentState{nullptr},
                                          previousState{nullptr},
                                          globalState{nullptr}
  {
  }

  virtual ~EntityStateMachine() {}

  void update()
  {

    if (globalState != nullptr)
    {
      globalState->execute(m_owner);
    }

    if (currentState != nullptr)
    {
      currentState->execute(m_owner);
    }
  }

  void changeState(State<EntityType> *newState)
  {
    printf("Changing state\n");
    assert(newState && "changing to null state");
    printf("assertion valid\n");

    previousState = currentState;

    // Check if this is the first time this is called
    if (currentState != nullptr)
    {
      currentState->onExit(m_owner);
    }
    currentState = newState;
    currentState->onEnter(m_owner);
  }

  void revertToPreviousState()
  {
    changeState(previousState);
  }
};
