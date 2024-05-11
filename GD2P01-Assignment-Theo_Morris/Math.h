#pragma once

#include <SFML/Graphics.hpp>

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

inline void normalize(sf::Vector2f& vector)
{
    float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (magnitude != 0) {
        vector /= magnitude;
    }
}