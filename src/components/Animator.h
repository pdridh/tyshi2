#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <algorithm>
#include <cassert>

#include "../utils/Vec2.h"

struct Animation
{
  // The rect to draw from the texture (source rect)
  SDL_Rect frameRect;
  // The x'th number of frame to start from (not in actual pixels but frames)
  int startX = 0;
  // The y'th number of frame to start from (not in actual pixels but frames)
  int startY = 0;
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
  SDL_Texture *m_sheet;
  std::vector<Animation *> m_animations;
  Vec2 m_scale;
  int m_currentAnimation;
  SDL_RendererFlip m_renderFlipped;

  Animator(SDL_Texture *sheet, int nAnimations, Vec2 scale)
      : m_sheet{sheet}, m_animations{nAnimations, nullptr},
        m_scale{scale}, m_currentAnimation{0}, m_renderFlipped{SDL_FLIP_NONE}
  {
  }

  void addAnimation(int id, int startX, int startY,
                    int frames, int frameWidth,
                    int frameHeight, double fps,
                    double angle)
  {
    assert(isAssigned(id));

    Animation *newAnimation = new Animation();
    newAnimation->startX = startX;
    newAnimation->startY = startY;
    newAnimation->frameRect.x = startX * frameWidth;
    newAnimation->frameRect.y = startY * frameHeight;
    newAnimation->frameRect.w = frameWidth;
    newAnimation->frameRect.h = frameHeight;

    newAnimation->frames = frames;
    newAnimation->currentFrame = 0;
    newAnimation->frameTime = 0.0;
    newAnimation->frameDuration = 1 / fps; // calculate duration of each animation from fps

    newAnimation->angle = angle;

    m_animations[id] = newAnimation;
  }

  bool isAssigned(int id)
  {
    Animation *found = m_animations[id];
    return found;
  }

  void reset(int id)
  {
    assert(!isAssigned(id));

    Animation *anim = m_animations[id];
    anim->frameRect.x = anim->startX * anim->frameRect.w;
    anim->frameRect.y = anim->startY * anim->frameRect.h;
    anim->currentFrame = 0;
    anim->frameTime = 0.0;
  }
};