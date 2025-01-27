#include <SDL2/SDL.h>

#include "core/Engine.hpp"

#include <random>
#include <time.h>

int main()
{
  srand(time(NULL));
  Engine *eng = new Engine("Tyshi");

  return 0;
}