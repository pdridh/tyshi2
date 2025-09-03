#pragma once

#include "Component.h"
#include <unordered_map>
#include <string>
#include "../../../utils/Vec2.h"

struct Animation
{
  // The x'th number of frame to start from (not in actual pixels but frames)
  int startX = 0;
  // The y'th number of frame to start from (not in actual pixels but frames)
  int startY = 0;
  // The number of frames that this animation has
  int frames;
  // The width of each frame
  int frameWidth;
  // The height of each frame
  int frameHeight;
  // The current frame that is being shown
  int currentFrame;
  // The duration of each frame
  double frameDuration;
  // The time elapsed by the current frame
  double frameTime;
  // Angle to rotate this animation with
  double angle;
};

struct Animator : public Component
{
  Animator(Entity *owner) : Component(owner) {}

  size_t textureSheetID;
  std::unordered_map<std::string, Animation> animations;
  std::string currentAnimationID;
  Vec2f scale;
  bool flipped;
};
