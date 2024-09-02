#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <algorithm>

#include "../utils/Vec2.h"

class Animator
{
public:
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

public:
  // Constructor currently takes a whole spritesheet (not sure)
  Animator(SDL_Texture *sheet, int animations, Vec2 scale = Vec2(1.0, 1.0));
  // Add an animation for the animator to calculate frames with
  void addAnimation(int id, int startX, int startY,
                    int frames, int frameWidth,
                    int frameHeight, double fps = 24,
                    double angle = 0.0);
  // Update animation frames + manage animations
  void update(const double dt);
  // Change the current animation
  void playAnimation(int id, SDL_RendererFlip flip = SDL_FLIP_NONE);
  // Draw at the given position (also dunno)
  void drawAt(Vec2 pos, SDL_Renderer *renderer);

private:
  // Helper fuction to check if id is assigned
  bool isAssigned(int id);
  // Reset an animation back to its start
  void reset(int id);

private:
  Vec2 m_scale;
  SDL_Texture *m_sheet;
  SDL_RendererFlip m_renderFlipped;
  int m_currentAnimation;
  std::vector<Animation *> m_animations;
};