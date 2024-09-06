#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <algorithm>

#include "../utils/Vec2.h"

struct Animation
{
  // The rect to draw from the texture (source rect)
  SDL_Rect frameRect;
  // The x'th number of frame to start from (not in actual pixels but frames)
  int startX;
  // The y'th number of frame to start from (not in actual pixels but frames)
  int startY;
  // The number of frames that this animation has
  int frames;
  // The current frame that is being shown
  int currentFrame;
  // The duration of each frame
  double frameDuration;
  // The time elapsed by the current frame
  double frameTime;
  // Angle to rotate this animation with
  double angle;
};

struct Animator
{
  Vec2 m_scale;
  SDL_Texture *m_sheet;
  SDL_RendererFlip m_renderFlipped;
  int m_currentAnimation;
  std::vector<Animation *> m_animations;
};