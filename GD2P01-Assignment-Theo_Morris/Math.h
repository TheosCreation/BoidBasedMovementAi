#pragma once

#include <SFML/System/Vector2.hpp>

// Declare the function as inline to avoid linker errors
inline void wrapPosition(sf::Vector2f& position, const sf::Vector2u& windowSize) {
    // Wrap X position
    if (position.x < 0)
        position.x = windowSize.x;
    else if (position.x > windowSize.x)
        position.x = 0;

    // Wrap Y position
    if (position.y < 0)
        position.y = windowSize.y;
    else if (position.y > windowSize.y)
        position.y = 0;
}

inline sf::Vector2f normalize(sf::Vector2f vector)
{
    sf::Vector2f normalisedVector;
    float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (magnitude != 0) {
        normalisedVector = vector / magnitude;
    }
    return normalisedVector;
}

inline float vectorMagnitude(sf::Vector2f vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

inline float vectorDistance(sf::Vector2f vectorA, sf::Vector2f vectorB)
{
    float deltaX = vectorB.x - vectorA.x;
    float deltaY = vectorB.y - vectorA.y;
    return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}