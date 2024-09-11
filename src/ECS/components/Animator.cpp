#include "Animator.h"

Animator::Animator(Entity *parent, SDL_Texture *sheet, size_t nAnimations, Vec2 scale)
    : Component{parent}, m_sheet{sheet}, m_animations{nAnimations, nullptr}, m_animationsAdded{0},
      m_scale{scale}, m_currentAnimation{0}, m_renderFlipped{SDL_FLIP_NONE}
{
}

void Animator::addAnimation(int id, int startX, int startY,
                            int frames, int frameWidth,
                            int frameHeight, double fps,
                            double angle)
{

  assert(id >= 0 && id < m_animations.size());
  assert(!isAssigned(id));
  assert(m_animationsAdded < m_animations.size());

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
  ++m_animationsAdded;
}

void Animator::playAnimation(int id, SDL_RendererFlip flip)
{
  // Already playing this animation
  if (id == m_currentAnimation && flip == m_renderFlipped)
  {
    return;
  }

  // Can't play an animation if it doesnt exist
  assert(isAssigned(id));

  reset(id);
  m_renderFlipped = flip;
  m_currentAnimation = id;
}

void Animator::update(const double dt)
{
  assert(m_animationsAdded > 0);

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

void Animator::draw(SDL_Renderer *renderer)
{
  assert(m_animationsAdded > 0);
  assert(m_parent->hasComponent<Transform>());

  Animation *currentAnim = m_animations[m_currentAnimation];
  Transform &transform = m_parent->getComponent<Transform>();

  SDL_FRect dstRect;
  dstRect.w = currentAnim->frameRect.w * m_scale.x,
  dstRect.h = currentAnim->frameRect.h * m_scale.y;
  // Position is center so box should be started from top-left (half of width)
  dstRect.x = transform.position.x - (dstRect.w / 2.f);
  dstRect.y = transform.position.y - (dstRect.h / 2.f);
  SDL_FPoint center = {(float)transform.position.x, (float)transform.position.y};

  SDL_RenderCopyExF(renderer, m_sheet,
                    &currentAnim->frameRect, &dstRect,
                    currentAnim->angle, &center, m_renderFlipped);
}

bool Animator::isAssigned(int id)
{
  assert(id >= 0 && id < m_animations.size());

  Animation *found = m_animations[id];
  return found;
}

void Animator::reset(int id)
{
  assert(id >= 0 && id < m_animations.size());
  assert(isAssigned(id));

  Animation *anim = m_animations[id];
  anim->frameRect.x = anim->startX * anim->frameRect.w;
  anim->frameRect.y = anim->startY * anim->frameRect.h;
  anim->currentFrame = 0;
  anim->frameTime = 0.0;
}