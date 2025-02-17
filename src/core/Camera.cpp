#include "Camera.h"

Camera::Camera(SDL_Renderer *renderer, f32 width, f32 height)
    : m_renderer{renderer}, m_width{width}, m_height{height}, zoom{5.f}
{
  centerOn(Vec2f::ZERO());
}

void Camera::centerOn(Vec2f centerPosInWorld)
{
  m_center = centerPosInWorld;
}

Vec2f Camera::screenToWorld(Vec2f screenPos)
{
  Vec2f worldPos = screenPos - Vec2f(m_width / 2, m_height / 2);
  worldPos.scaleDown(zoom);
  worldPos.y *= -1;
  worldPos += m_center;
  return worldPos;
}

Vec2f Camera::worldToScreen(Vec2f worldPos)
{
  Vec2f screenPos = worldPos - m_center;
  screenPos.y *= -1;
  screenPos.scale(zoom);
  screenPos += Vec2f(m_width / 2, m_height / 2);
  return screenPos;
}

void Camera::drawRect(Vec2f worldPos, f32 width, f32 height, Color color, bool filled)
{
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

  width = width * zoom;
  height = height * zoom;

  Vec2f screenPos = worldToScreen(worldPos);

  // Everything is centered so offset
  screenPos -= Vec2f(width / 2, height / 2);
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
