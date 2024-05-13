/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Obstacle.cpp
Description : Implements the Obstacle class, which represents an obstacle in the game.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f position, float radius) : m_pos(position), m_radius(radius)
{
    // Load texture, set sprite texture, and initial sprite setup
    if (!m_texture.loadFromFile("Assets/Textures/Obstacle.png")) {
        std::cerr << "Failed to load texture from 'Assets/Textures/Obstacle.png'" << std::endl;
    }

    m_sprite.setTexture(m_texture);
    m_sprite.setOrigin(m_texture.getSize().x / 2.0f, m_texture.getSize().y / 2.0f);
    m_sprite.setPosition(m_pos);

    // Calculate the obstacle diameter (2 * radius) which is scaled with texture size 
    float scaleX = (2 * radius) / m_texture.getSize().x;
    float scaleY = (2 * radius) / m_texture.getSize().y;
    m_sprite.setScale(scaleX, scaleY);
}

Obstacle::~Obstacle()
{
}

void Obstacle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}