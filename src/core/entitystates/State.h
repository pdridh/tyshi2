#pragma once

template <class EntityType>
class State
{
public:
  virtual ~State() {}

  virtual void onEnter(EntityType &) = 0;
  virtual void execute(EntityType &) = 0;
  virtual void onExit(EntityType &) = 0;
};
