#pragma once

#include <SDL3/SDL.h>

#include "../utils/Vec2.h"

struct Color
{
  u32 r = 255;
  u32 g = 255;
  u32 b = 255;
  u32 a = 255;
};

class Camera
{
private:
  Vec2f m_center;
  SDL_Renderer *m_renderer;
  f32 m_width;
  f32 m_height;

public:
  Camera(SDL_Renderer *renderer, f32 width, f32 height);
  void centerOn(Vec2f centerPos);
  Vec2f screenToWorld(Vec2f screenPos);
  Vec2f worldToScreen(Vec2f worldPos);
  void drawRect(Vec2f worldPos, f32 width, f32 height, Color color, bool filled = false);
  void drawRect(f32 worldX, f32 worldY, f32 width, f32 height, Color color, bool filled = false);
  void drawTexture(SDL_Texture *texture, SDL_FRect &srcRect, Vec2f worldPos, f32 width, f32 height, bool trans);
};
