#include "Animator.h"

Animator::Animator(SDL_Texture *sheet, int animations, Vec2 scale)
{
  m_sheet = sheet;
  m_animations.reserve(animations);
  m_animations = {nullptr};
  m_scale = scale;
}

void Animator::addAnimation(int id, int startX, int startY,
                            int frames, int frameWidth,
                            int frameHeight, double fps,
                            SDL_RendererFlip flip, double angle)
{
  if (isAssigned(id))
  {
    printf("Animator::addAnimation - ID already assigned to an animation\n");
    return;
  }

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

  newAnimation->flip = flip;
  newAnimation->angle = angle;

  m_animations[id] = newAnimation;
  m_currentAnimation = m_animations.size() - 1;
}

void Animator::update(const double dt)
{
  // Change animations here
  // + calculate animation stuff

  if (m_animations.empty())
  {
    return;
  }

  Animation *currentAnim = m_animations[m_currentAnimation];
  currentAnim->frameTime += dt;
  // Update this animation
  if (currentAnim->frameTime >= currentAnim->frameDuration)
  {
    currentAnim->frameTime = 0;
    currentAnim->currentFrame++;
    currentAnim->currentFrame = currentAnim->currentFrame % currentAnim->frames;
    currentAnim->frameRect.x = (currentAnim->startX * currentAnim->frameRect.w) +
                               currentAnim->frameRect.w * currentAnim->currentFrame;
  }
}

void Animator::playAnimation(int id)
{
  // Already playing this animation
  if (id == m_currentAnimation)
  {
    return;
  }

  if (!isAssigned(id))
  {
    printf("Animation::remove - Tried playing unkown animation\n");
    return;
  }

  reset(id);
  m_currentAnimation = id;
}

void Animator::drawAt(Vec2 pos, SDL_Renderer *renderer)
{
  if (m_animations.empty())
  {
    return;
  }

  Animation *currentAnim = m_animations[m_currentAnimation];

  SDL_FRect dstRect;
  dstRect.w = currentAnim->frameRect.w * m_scale.x,
  dstRect.h = currentAnim->frameRect.h * m_scale.y;
  dstRect.x = pos.x - (dstRect.w / 2);
  dstRect.y = pos.y - (dstRect.h / 2);
  SDL_FPoint center = {pos.x, pos.y};

  SDL_RenderCopyExF(renderer, m_sheet,
                    &currentAnim->frameRect, &dstRect,
                    currentAnim->angle, &center, currentAnim->flip);
}

bool Animator::isAssigned(int id)
{
  Animation *found = m_animations[id];
  return (bool)found;
}

void Animator::reset(int id)
{
  if (!isAssigned(id))
  {
    printf("Animator::reset - resetting unkown animation (shouldnt be possible)\n");
    return;
  }

  Animation *anim = m_animations[id];
  anim->frameRect.x = anim->startX * anim->frameRect.w;
  anim->frameRect.y = anim->startY * anim->frameRect.h;
  anim->currentFrame = 0;
  anim->frameTime = 0.0;
}
