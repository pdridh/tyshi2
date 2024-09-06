#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../utils/Vec2.h"

// Wrapper to hold and draw texture, the part of the texture to draw
// A box surrounding the texture, angle and flip of texture etc..
class Sprite
{
public:
  SDL_Texture *texture = nullptr;
  SDL_Rect textureRect{0, 0, 0, 0};
  Vec2 scale{1, 1};
  SDL_FRect destinationRect{0, 0, 0, 0};
  double rotateAngle{0};
  SDL_RendererFlip flip{SDL_FLIP_NONE};
  bool debug{false};
  SDL_Color debugBoxColor{255, 0, 0, 255};
};