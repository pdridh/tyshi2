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
  Camera(SDL_Renderer *renderer, float width, float height)
      : m_renderer{renderer}, m_width{width}, m_height{height}
  {
  }

  void centerOn(Vec2 centerPos)
  {
    m_center = centerPos - Vec2(m_width / 2, m_height / 2);
  }

  Vec2 screenToWorld(Vec2 screenPos)
  {
    return screenPos + m_center;
  }

  Vec2 worldToScreen(Vec2 worldPos)
  {
    return worldPos - m_center;
  }

  void drawRect(Vec2 worldPos, float width, float height, Color color)
  {
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

    // Everything is centered
    Vec2 finalPos = worldToScreen(Vec2(worldPos.x - width / 2, worldPos.y - height / 2));
    SDL_FRect rect = {finalPos.x,
                      finalPos.y,
                      width,
                      height};

    SDL_RenderDrawRectF(m_renderer, &rect);
  }
  void drawRect(float worldX, float worldY, float width, float height, Color color)
  {
    drawRect(Vec2(worldX, worldY), width, height, color);
  }
};
