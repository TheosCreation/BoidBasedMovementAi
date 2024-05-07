#include "Agent.h"

Agent::Agent(int spawnPositionX, int spawnPositionY)
{
    // Load texture for the sprite
    sf::Texture texture;
    std::string textureFilePath = "Assets/Textures/Wander.png";
    if (!texture.loadFromFile(textureFilePath)) {
        // Handle error loading texture
        std::cout << "Error Loading Texture: " << textureFilePath << std::endl;
    }

    // Set texture for the sprite
    m_sprite.setTexture(texture);
    // Set position and size of the sprite
    m_sprite.setPosition(spawnPositionX - m_sprite.getLocalBounds().width / 2, spawnPositionY - m_sprite.getLocalBounds().height / 2);
}

Agent::~Agent()
{
}

void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}
