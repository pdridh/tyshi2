#pragma once

#include <SDL2/SDL.h>

#include "../utils/Vec2.h"

struct Color
{
  unsigned int r = 255;
  unsigned int g = 255;
  unsigned int b = 255;
  unsigned int a = 255;
};

class Camera
{
private:
  Vec2 m_center;
  SDL_Renderer *m_renderer;
  float m_width;
  float m_height;

public:
  Camera(SDL_Renderer *renderer, float width, float height);
  void centerOn(Vec2 centerPos);
  Vec2 screenToWorld(Vec2 screenPos);
  Vec2 worldToScreen(Vec2 worldPos);
  void drawRect(Vec2 worldPos, float width, float height, Color color);
  void drawRect(float worldX, float worldY, float width, float height, Color color);
  void drawTexture(SDL_Texture *texture, SDL_Rect &srcRect, Vec2 worldPos, float width, float height);
};
