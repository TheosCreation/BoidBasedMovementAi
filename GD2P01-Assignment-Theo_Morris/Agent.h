#pragma once

#include <iostream>
#include <random>
#include "Math.h"

#include <SFML/Graphics.hpp>

// Use a global random number engine and distribution
static std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
static std::uniform_real_distribution<float> distribution(0.0f, 360.0f);

const float PI = 3.14159265359f;
const float NEIGHBOR_RADIUS = 100.0f;
const float SEPARATION_RADIUS = 50.0f;
const float MAX_SPEED = 0.1f;
const float MAX_FORCE = 0.01f;

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
    sf::Vector2f m_velocity;
    float m_rot;
    float m_acceleration;

public:
    Agent(int spawnPositionX, int spawnPositionY);
    ~Agent();

    // Override the draw function to make the agent drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void update(const sf::Vector2u& windowSize, const std::vector<std::unique_ptr<Agent>>& agents);
    sf::Vector2f seek(const sf::Vector2f& target);
    sf::Vector2f separate(const std::vector<std::unique_ptr<Agent>>& agents);

    sf::Vector2f getPosition() const { return m_pos; }
    sf::Vector2f getVelocity() const { return m_velocity; }
};