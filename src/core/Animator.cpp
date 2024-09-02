#include "Animator.h"

void Animator::Animation::reset()
{
  frameRect.x = startX * frameRect.w;
  frameRect.y = startY * frameRect.h;
}

Animator::Animator(SDL_Texture *sheet)
{
  m_sheet = sheet;
}

void Animator::addAnimation(int startX, int startY, int frames,
                            int frameWidth, int frameHeight, double fps,
                            Vec2 scale, double angle, SDL_RendererFlip flip)
{
  Animation *newAnimation = new Animation();
  newAnimation->startX = startX;
  newAnimation->startY = startY;
  newAnimation->frameRect.x = startX * frameWidth;
  newAnimation->frameRect.y = startY * frameHeight;
  newAnimation->frameRect.w = frameWidth;
  newAnimation->frameRect.h = frameHeight;

  newAnimation->frames = frames;

  newAnimation->frameDuration = 1 / fps; // calculate duration of each animation from fps

  newAnimation->scale = scale;
  newAnimation->angle = angle;
  newAnimation->flip = flip;

  m_animations.push_back(newAnimation);
  m_currentAnimation = m_animations.size() - 1;

  m_currentAnimFrame = 0;
  m_currentFrameTime = 0.0;
}

void Animator::update(const double dt)
{
  // Change animations here
  // + calculate animation stuff

  if (m_animations.empty())
  {
    return;
  }

  m_currentFrameTime += dt;
  // Update this animation
  Animation *currentAnim = m_animations[m_currentAnimation];
  if (m_currentFrameTime >= currentAnim->frameDuration)
  {
    m_currentFrameTime = 0;
    m_currentAnimFrame = (m_currentAnimFrame + 1) % currentAnim->frames;
    currentAnim->frameRect.x = (currentAnim->startX * currentAnim->frameRect.w) + currentAnim->frameRect.w * m_currentAnimFrame;
  }
}

void Animator::playAnimation(int id)
{
  // Already playing this animation
  if (id == m_currentAnimation)
  {
    return;
  }

  Animation *found = m_animations[id];
  if (!found)
  {
    printf("Animation::remove - Tried removing unkown animation\n");
    return;
  }
  found->reset();

  m_currentAnimation = id;
  m_currentAnimFrame = 0;
  m_currentFrameTime = 0.0;
}

void Animator::drawAt(Vec2 pos, SDL_Renderer *renderer)
{
  if (m_animations.empty())
  {
    return;
  }

  Animation *currentAnim = m_animations[m_currentAnimation];

  SDL_FRect dstRect;
  dstRect.w = currentAnim->frameRect.w * currentAnim->scale.x,
  dstRect.h = currentAnim->frameRect.h * currentAnim->scale.y;
  dstRect.x = pos.x - (dstRect.w / 2);
  dstRect.y = pos.y - (dstRect.h / 2);
  SDL_FPoint center = {pos.x, pos.y};

  SDL_RenderCopyExF(renderer, m_sheet,
                    &currentAnim->frameRect, &dstRect,
                    currentAnim->angle, &center, currentAnim->flip);
}
