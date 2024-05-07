#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

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
    sf::Sprite m_sprite;
public:
    Agent(int spawnPositionX, int spawnPositionY);
    ~Agent();

    // Override the draw function to make the agent drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};