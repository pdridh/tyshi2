#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <algorithm>
#include <cassert>

#include "Component.h"
#include "../Entity.h"

/**
 * @brief    Holds the information that describes a single animation
 */
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

class Animator : public Component
{

public:
  /**
   * @brief Construct a new Animator object, currently only works with a sprite sheet
   *        that has all the animations for an entity
   *
   * @param sheet The texture sheet
   * @param nAnimations
   * @param scale
   */

  /**
   * @brief    Construct a new Animator object, currently only works with a sprite sheet
   *           that has all the animations for an entity
   * @param    parent                        The parent entity of this component
   * @param    sheet                         The texture sheet that has all the animations
   * @param    nAnimations                   The number of animations the sheet has or is used (for resizing the vector at start)
   * @param    scale                         The factor to scale each frame by
   */
  Animator(Entity *parent, SDL_Texture *sheet, size_t nAnimations, Vec2 scale = Vec2(1.0, 1.0));

  /**
   * @brief    Add an animation by describing the frames in the sheet provided to this Animator
   *
   * @param    id                            The id that will be used to reference this animation.
   *                                         Has to be a valid id (less than the number of animations provided to constructor and greater than zero)
   *                                         that is not already assigned to an animation
   * @param    startX                        The x'th position of the start of the animation in the sprite sheet
   * @param    startY                        The y'th position of the start of the animation in the sprite sheet
   * @param    frames                        The number of frames the animation has in the sprite sheet from the startX, startY position
   * @param    frameWidth                    The width of a single frame
   * @param    frameHeight                   The height of a single frame
   * @param    fps                           The speed (frame per second) that this animation will play in
   * @param    angle                         The angle that the animation is rotated by (Cant think of a use but adding it anyways)
   */
  void addAnimation(int id, int startX, int startY, int frames,
                    int frameWidth, int frameHeight, double fps,
                    double angle = 0.0);

  /**
   * @brief    Change the animation to be played using an id
   *
   * @param    id                            The id of the animation to be played (doesnt do anything if the id is same as the current animation being played)
   * @param    flip                          Flag if the animation should be played flipped
   */
  void playAnimation(int id, SDL_RendererFlip flip = SDL_FLIP_NONE);

  /**
   * @brief    Update the frames of the current animation
   *
   * @param    dt                            Time for this frame
   */
  void update(const double dt) override final;

  /**
   * @brief    Draw the current frame of the animation using the transform component of the parent
   *
   * @param    renderer                      Renderer to draw shit with
   */
  void draw(SDL_Renderer *renderer) override final;

private:
  /**
   * @brief    Helper function that checks if an animation has already been created for the given id
   *
   * @param    id                            The id of the animation to check
   * @return   true                          If the animation is not null therefore already created
   * @return   false                         If the animation is null therefore not created
   */
  bool isAssigned(int id);

  /**
   * @brief    Helper function to reset an animation, ie. change its value to the intial value (frameTime, currentFrame, etc..)
   *
   * @param    id                            The id to reference the animation
   */
  void reset(int id);

private:
  SDL_Texture *m_sheet;
  std::vector<Animation *> m_animations;
  int m_animationsAdded;
  Vec2 m_scale;
  int m_currentAnimation;
  SDL_RendererFlip m_renderFlipped;
};