#pragma once

#include "State.h"
#include <iostream>

class Player;

class PlayerIdle : public State<Player>
{
private:
  PlayerIdle() {}

  // copy ctor and assignment should be private
  PlayerIdle(const PlayerIdle &);
  PlayerIdle &operator=(const PlayerIdle &);

public:
  static PlayerIdle *instance();

  virtual void onEnter(Player &player) override;
  virtual void execute(Player &player) override;
  virtual void onExit(Player &player) override;
};