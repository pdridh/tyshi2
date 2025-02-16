#include "Camera.h"

Camera::Camera(SDL_Renderer *renderer, f32 width, f32 height)
    : m_renderer{renderer}, m_width{width}, m_height{height}
{
  centerOn(Vec2f::ZERO());
}

void Camera::centerOn(Vec2f centerPos)
{
  m_center = centerPos - Vec2f(m_width / 2, m_height / 2);
}

Vec2f Camera::screenToWorld(Vec2f screenPos)
{
  Vec2f world = screenPos + m_center;
  world.y *= -1;
  return world;
}

Vec2f Camera::worldToScreen(Vec2f worldPos)
{
  return worldPos - m_center;
}

void Camera::drawRect(Vec2f worldPos, f32 width, f32 height, Color color, bool filled)
{
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

  // Everything is centered
  Vec2f screenPos = worldToScreen(Vec2f(worldPos.x - width / 2, worldPos.y - height / 2));
  SDL_FRect rect = {screenPos.x,
                    screenPos.y,
                    width,
                    height};
  if (filled)
  {
    SDL_RenderFillRect(m_renderer, &rect);
    return;
  }

  SDL_RenderRect(m_renderer, &rect);
}

void Camera::drawRect(f32 worldX, f32 worldY, f32 width, f32 height, Color color, bool filled)
{
  drawRect(Vec2f(worldX, worldY), width, height, color, filled);
}

void Camera::drawTexture(SDL_Texture *texture, SDL_FRect &srcRect, Vec2f worldPos, f32 width, f32 height, bool trans)
{
  Vec2f screenPos = worldToScreen(Vec2f(worldPos.x - width / 2, worldPos.y - height / 2));
  SDL_FRect dstRect = {screenPos.x, screenPos.y, width, height};
  if (trans)
  {
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
  }
  SDL_RenderTexture(m_renderer, texture, &srcRect, &dstRect);
}
