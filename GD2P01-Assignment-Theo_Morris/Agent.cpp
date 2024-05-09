#include "Agent.h"

Agent::Agent(int spawnPositionX, int spawnPositionY)
{
    m_texture.loadFromFile("Assets/Textures/Wander.png");
    // Set texture for the sprite
    m_sprite.setTexture(m_texture);

    // Set the scale of the sprite to be accurate for the world
    m_sprite.setScale(0.1f, 0.1f);

    // Set position and size of the sprite
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);

    // Set position of the sprite so that its center aligns with (spawnPositionX, spawnPositionY)
    m_sprite.setPosition(spawnPositionX, spawnPositionY);

}

Agent::~Agent()
{
}

void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}
