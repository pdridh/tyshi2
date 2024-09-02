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
    SDL_Rect frameRect;
    int startX;
    int startY;
    int frames;
    double frameDuration;
    Vec2 scale;
    double angle;
    SDL_RendererFlip flip;

    // Reset animation back to its start
    void reset();
  };

public:
  // Constructor currently takes a whole spritesheet (not sure)
  Animator(SDL_Texture *sheet);
  // Add an animation for the animator to calculate frames with
  void addAnimation(int startX, int startY,
                    int frames, int frameWidth, int frameHeight,
                    double fps = 24, Vec2 scale = Vec2(1.0, 1.0),
                    double angle = 0.0, SDL_RendererFlip flip = SDL_FLIP_NONE);
  // Update animation frames + manage animations
  void update(const double dt);
  // Change the current animation
  void playAnimation(int id);
  // Draw at the given position (also dunno)
  void drawAt(Vec2 pos, SDL_Renderer *renderer);

private:
  SDL_Texture *m_sheet;
  int m_currentAnimation;
  int m_currentAnimFrame;
  double m_currentFrameTime;
  std::vector<Animation *> m_animations;
};