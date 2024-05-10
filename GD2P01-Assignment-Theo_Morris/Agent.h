#pragma once

#include <iostream>
#include <random>
#include "Math.h"

#include <SFML/Graphics.hpp>

// Use a global random number engine and distribution
static std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
static std::uniform_real_distribution<float> distribution(0.0f, 360.0f);

enum class MovementBehavior {
    Seek,
    Pursue,
    Wander,
    Arrival,
    Pathing,
    Flocking,
    LeaderFollow,
    CrowdPathing
};

class Agent : public sf::Drawable
{
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    sf::Vector2f m_pos;
    float m_rot = 0;

    sf::Vector2f m_velocity;
    float m_acceleration = 0.01;
public:
    Agent(int spawnPositionX, int spawnPositionY);
    ~Agent();

    // Override the draw function to make the agent drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void update(const sf::Vector2u& windowSize);
};