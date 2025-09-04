#include "PlayState.h"
#include "MenuState.h"
#include "../ecs/components/Controller.h"
#include "../ecs/components/Animator.h"
#include "../ecs/systems/InputSystem.h"
#include "../ecs/systems/MovementSystem.h"
#include "../ecs/systems/AnimationSystem.h"

PlayState PlayState::m_self;

PlayState *PlayState::instance()
{
  return &m_self;
}

void PlayState::onEnter(Engine &engine)
{
  printf("Entering play...\n");
  world = new World(engine);
  entityManager = new EntityManager();

  // TODO load all this from some data file (XML probably?)
  Entity &player = entityManager->addEntity();
  Transform &transform = player.getComponent<Transform>();
  player.addComponent<Controller>();
  transform.position = Vec2f(100, 100);
  Animator &anim = player.addComponent<Animator>();

  Animation idle;
  idle.startX = 0;
  idle.startY = 0;
  idle.frames = 4;
  idle.frameDuration = 0.4;
  idle.currentFrame = 0;
  idle.frameWidth = 32;
  idle.frameHeight = 32;
  idle.frameTime = 0;
  idle.angle = 1.0;

  anim.animations["idle"] = idle;
  anim.currentAnimationID = "idle";
  anim.scale = Vec2f(1.0, 1.0);
  anim.textureSheetID = engine.resourceManager->loadTexture("assets/cow.png");
  anim.flipped = false;
}

void PlayState::onExit(Engine &engine)
{
  printf("Exiting play...\n");
  delete world;
  world = nullptr;
}

void PlayState::pause(Engine &engine)
{
}

void PlayState::resume(Engine &engine)
{
}

void PlayState::update(Engine &engine)
{
  if (engine.input->isKeyDownFrame(SDL_SCANCODE_ESCAPE))
  {
    engine.changeState(MenuState::instance());
  }

  i32 scroll = engine.input->mouseScroll();
  if (scroll < 0)
  {
    engine.camera->zoomBy(0.75);
  }
  else if (scroll > 0)
  {
    engine.camera->zoomBy(1.25);
  }

  if (world)
  {
    world->update();
  }

  entityManager->refresh();

  InputSystem(entityManager, engine.input);
  MovementSystem(entityManager, engine.dt);
  AnimationSystem(entityManager, engine.dt);
}

void PlayState::draw(Engine &engine)
{
  RenderAnimationSystem(entityManager, engine.resourceManager, engine.camera);
  // Draw world?
}
