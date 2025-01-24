#include "Camera.h"

Camera::Camera(SDL_Renderer *renderer, float width, float height)
    : m_renderer{renderer}, m_width{width}, m_height{height}
{
  centerOn(Vec2::ZERO());
}

void Camera::centerOn(Vec2 centerPos)
{
  m_center = centerPos - Vec2(m_width / 2, m_height / 2);
}

Vec2 Camera::screenToWorld(Vec2 screenPos)
{
  Vec2 world = screenPos + m_center;
  world.y *= -1;
  return world;
}

Vec2 Camera::worldToScreen(Vec2 worldPos)
{
  return worldPos - m_center;
}

void Camera::drawRect(Vec2 worldPos, float width, float height, Color color)
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

void Camera::drawRect(float worldX, float worldY, float width, float height, Color color)
{
  drawRect(Vec2(worldX, worldY), width, height, color);
}

void Camera::drawTexture(SDL_Texture *texture, SDL_Rect &srcRect, Vec2 worldPos, float width, float height)
{
  Vec2 screenPos = worldToScreen(worldPos);
  SDL_FRect dstRect = {screenPos.x, screenPos.y, width, height};

  SDL_RenderCopyExF(m_renderer, texture, &srcRect, &dstRect, 0, NULL, SDL_FLIP_NONE);
}
