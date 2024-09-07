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
  SDL_Texture *sheet;
  std::vector<Animation *> animations;
  int animationsAdded;
  Vec2 scale;
  int currentAnimation;
  SDL_RendererFlip renderFlipped;

  Animator(SDL_Texture *sheet_arg, int nAnimations_arg, Vec2 scale_arg = Vec2(1.f, 1.f))
      : sheet{sheet_arg}, animations{nAnimations_arg, nullptr}, animationsAdded{0},
        scale{scale_arg}, currentAnimation{0}, renderFlipped{SDL_FLIP_NONE}
  {
  }

  void addAnimation(int id, int startX, int startY,
                    int frames, int frameWidth,
                    int frameHeight, double fps,
                    double angle)
  {
    assert(isAssigned(id));
    assert(animationsAdded < animations.size() - 1);

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

    animations[id] = newAnimation;
    ++animationsAdded;
  }

  void Animator::playAnimation(int id, SDL_RendererFlip flip)
  {
    // Already playing this animation
    if (id == currentAnimation && flip == renderFlipped)
    {
      return;
    }

    if (!isAssigned(id))
    {
      printf("Animation::remove - Tried playing unkown animation\n");
      return;
    }

    reset(id);
    renderFlipped = flip;
    currentAnimation = id;
  }

  bool isAssigned(int id)
  {
    Animation *found = animations[id];
    return found;
  }

  void reset(int id)
  {
    assert(!isAssigned(id));

    Animation *anim = animations[id];
    anim->frameRect.x = anim->startX * anim->frameRect.w;
    anim->frameRect.y = anim->startY * anim->frameRect.h;
    anim->currentFrame = 0;
    anim->frameTime = 0.0;
  }
};