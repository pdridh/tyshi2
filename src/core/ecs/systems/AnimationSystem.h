#pragma once

#include "../EntityManager.h"
#include "../../ResourceManager.h"
#include "../../Camera.h"
#include "../components/Animator.h"

void AnimationSystem(EntityManager *manager, f32 dt)
{

  for (auto &e : manager->m_entities)
  {

    // TODO implement better way to retrieve entities based on the components they have
    if (!e->hasComponent<Animator>())
    {
      continue;
    }

    // Do animation
    Animator &anim = e->getComponent<Animator>();

    Animation &currentAnim = anim.animations[anim.currentAnimationID];
    currentAnim.frameTime += dt;
    // Update this animation
    if (currentAnim.frameTime >= currentAnim.frameDuration)
    {
      currentAnim.frameTime = 0;
      currentAnim.currentFrame++;
      currentAnim.currentFrame = currentAnim.currentFrame % currentAnim.frames;
    }
  }
}

void RenderAnimationSystem(EntityManager *manager, ResourceManager *resourceManager, Camera *camera)
{
  for (auto &e : manager->m_entities)
  {
    // TODO implement better way to retrieve entities based on the components they have
    if (!e->hasComponent<Animator>())
    {
      continue;
    }

    Transform &transform = e->getComponent<Transform>();
    Animator &animator = e->getComponent<Animator>();
    Animation &currentAnim = animator.animations[animator.currentAnimationID];

    SDL_Texture *texture = resourceManager->getTextureById(animator.textureSheetID);

    int frameX = (currentAnim.startX * currentAnim.frameWidth) +
                 (currentAnim.currentFrame * currentAnim.frameWidth);
    int frameY = currentAnim.startY * currentAnim.frameHeight;

    SDL_FRect srcRect = {frameX,
                         frameY,
                         currentAnim.frameWidth,
                         currentAnim.frameHeight};

    // TODO use flipped (TODO for Camera tbh)
    camera->drawTexture(transform.position, texture, srcRect,
                        currentAnim.frameWidth * animator.scale.x,
                        currentAnim.frameHeight * animator.scale.y, false);
  }
}