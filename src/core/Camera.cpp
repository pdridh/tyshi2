#include "Camera.hpp"

Camera::Camera(SDL_Renderer *renderer, float width, float height)
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

void Camera::drawRect(Vec2f worldPos, float width, float height, Color color)
{
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

  // Everything is centered
  Vec2f screenPos = worldToScreen(Vec2f(worldPos.x - width / 2, worldPos.y - height / 2));
  SDL_FRect rect = {screenPos.x,
                    screenPos.y,
                    width,
                    height};

  SDL_RenderDrawRectF(m_renderer, &rect);
}

void Camera::drawRect(float worldX, float worldY, float width, float height, Color color)
{
  drawRect(Vec2f(worldX, worldY), width, height, color);
}

void Camera::drawTexture(SDL_Texture *texture, SDL_Rect &srcRect, Vec2f worldPos, float width, float height)
{
  Vec2f screenPos = worldToScreen(Vec2f(worldPos.x - width / 2, worldPos.y - height / 2));
  SDL_FRect dstRect = {screenPos.x, screenPos.y, width, height};

  SDL_RenderCopyExF(m_renderer, texture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);
}
