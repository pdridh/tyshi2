#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../utils/Vec2.h"

// Wrapper to hold and draw texture, the part of the texture to draw
// A box surrounding the texture, angle and flip of texture etc..
class Sprite
{
public:
  SDL_Texture *texture;
  SDL_Rect textureRect;
  Vec2 scale;
  SDL_FRect destinationRect;
  double rotateAngle;
  SDL_RendererFlip flip;
  bool debug;
  SDL_Color debugBoxColor;

public:
  // TODO MAYBE replace texture with an id of texture retrieved fro soe other context lowkey
  Sprite(SDL_Texture *texture, SDL_Rect textureRect)
      : texture{texture},
        textureRect{textureRect},
        scale{1, 1},
        destinationRect{0, 0, 0, 0},
        rotateAngle{0},
        flip{SDL_FLIP_NONE},
        debug{false},
        debugBoxColor{255, 0, 0, 255}
  {
  }

  void drawAt(Vec2 pos, SDL_Renderer *renderer)
  {
    SDL_FPoint center;
    center.x = pos.x;
    center.y = pos.y;

    // Position is center so box should be started fro top-left (half of width)
    destinationRect.w = textureRect.w * scale.x;
    destinationRect.h = textureRect.h * scale.y;
    destinationRect.x = pos.x - (destinationRect.w / 2.f);
    destinationRect.y = pos.y - (destinationRect.h / 2.f);

    if (debug)
    {
      SDL_SetRenderDrawColor(renderer, debugBoxColor.r, debugBoxColor.g, debugBoxColor.b, debugBoxColor.a);
      SDL_RenderDrawRectF(renderer, &destinationRect);
    }

    SDL_RenderCopyExF(renderer, texture, &textureRect, &destinationRect, rotateAngle, &center, flip);
  }
};