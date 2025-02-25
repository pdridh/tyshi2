#include "Player.h"
#include "World.h"

Player::Player(World &world, WorldPosition _position, size_t textureID)
    : m_world{world}, position{_position}, m_textureID{textureID}
{
  m_stateMachine = new EntityStateMachine<Player>(*this);
  m_stateMachine->changeState(PlayerIdle::instance());
}

Player::~Player()
{
  delete m_stateMachine;
}

void Player::update()
{
  m_stateMachine->update();
}

void Player::draw()
{
  SDL_FRect src = {0, 0, 32, 32};

  Engine &engine = m_world.m_engine;
  engine.camera->drawTexture(position.actualPosition,
                             engine.resourceManager->getTextureById(m_textureID),
                             src,
                             m_drawSize * 4, m_drawSize * 4, true);
}