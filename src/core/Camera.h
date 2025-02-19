#pragma once

#include "Core.h"

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
  f32 m_zoom;

public:
  Camera(SDL_Renderer *renderer, f32 width, f32 height);

  //****** CAMERA STUFF */
  void setCameraSize(f32 width, f32 height);
  void centerOn(Vec2f actualPosition);
  void setZoom(f32 zoom);
  void zoomBy(f32 scale);

  //************** CONVERSIONS */
  Vec2f screenToActual(Vec2f screenPosition);
  Vec2f actualToScreen(Vec2f actualPosition);

  //*********** DRAWING STUFF */
  void drawRect(Vec2f actualPosition, f32 width, f32 height, Color color, bool filled = false);
  void drawRect(f32 actualX, f32 actualY, f32 width, f32 height, Color color, bool filled = false);
  void drawTexture(Vec2f actualPosition, SDL_Texture *texture, SDL_FRect &srcRect, f32 width, f32 height, bool trans);
  void drawPoint(f32 actualX, f32 actualY, Color c);
};
