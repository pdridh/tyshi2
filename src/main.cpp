#include <SDL2/SDL.h>

int main()
{
  int rendererFlags, windowFlags;

  rendererFlags = SDL_RENDERER_ACCELERATED;

  windowFlags = 0;

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_Window *window = SDL_CreateWindow("Tyshi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, windowFlags);

  if (!window)
  {
    printf("Failed to open %d x %d window: %s\n", 800, 600, SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, rendererFlags);

  if (!renderer)
  {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_Delay(5000);

  return 0;
}