#include "Camera.h"

Camera::Camera(SDL_Renderer *renderer, f32 width, f32 height)
    : m_renderer{renderer}, m_width{width}, m_height{height}, m_zoom{1.f}
{
  centerOn(Vec2f::ZERO());
}

void Camera::setCameraSize(f32 width, f32 height)
{
  m_width = width;
  m_height = height;
}

void Camera::centerOn(Vec2f actualPosition)
{
  m_center = actualPosition;

  if (m_center.x < m_width / 2 / m_zoom)
    m_center.x = m_width / 2 / m_zoom;
  else if (m_center.x > 10000)
    m_center.x = 10000;

  if (m_center.y < m_height / 2 / m_zoom)
    m_center.y = m_height / 2 / m_zoom;
  else if (m_center.y > 10000)
    m_center.y = 10000;
}

void Camera::setZoom(f32 zoom)
{
  m_zoom = zoom;
}

void Camera::zoomBy(f32 scale)
{
  m_zoom *= scale;
}

Vec2f Camera::screenToActual(Vec2f screenPosition)
{
  Vec2f actualPosition = screenPosition - Vec2f(m_width / 2, m_height / 2);
  actualPosition.scaleDown(m_zoom);
  actualPosition.y *= -1;
  actualPosition += m_center;
  return actualPosition;
}

Vec2f Camera::actualToScreen(Vec2f actualPosition)
{
  Vec2f screenPosition = actualPosition - m_center;
  screenPosition.y *= -1;
  screenPosition.scale(m_zoom);
  screenPosition += Vec2f(m_width / 2, m_height / 2);
  return screenPosition;
}

void Camera::drawRect(Vec2f actualPosition, f32 width, f32 height, Color color, bool filled)
{
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

  width = width * m_zoom;
  height = height * m_zoom;

  Vec2f screenPosition = actualToScreen(actualPosition);

  // Everything is centered so offset
  screenPosition -= Vec2f(width / 2, height / 2);
  SDL_FRect rect = {screenPosition.x,
                    screenPosition.y,
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

void Camera::drawTexture(Vec2f actualPosition, SDL_Texture *texture, SDL_FRect &srcRect, f32 width, f32 height, bool trans)
{
  width = width * m_zoom;
  height = height * m_zoom;

  Vec2f screenPosition = actualToScreen(actualPosition);
  screenPosition -= Vec2f(width / 2, height / 2);

  SDL_FRect dstRect = {screenPosition.x, screenPosition.y, width, height};
  if (trans)
  {
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
  }

  SDL_RenderTexture(m_renderer, texture, &srcRect, &dstRect);
}

void Camera::drawPoint(f32 x, f32 y, Color c)
{
  Vec2f screenPosition = actualToScreen(Vec2f(x, y));
  SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a);
  SDL_RenderPoint(m_renderer, screenPosition.x, screenPosition.y);
}
