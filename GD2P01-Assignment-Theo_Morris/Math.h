/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Math.h
Description : Contains vector math functions that are applied to vectors
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#pragma once

#include <SFML/System/Vector2.hpp>
#include <limits>

const float PI = 3.14159265359f;

/***
 * Function to wrap the position vector within the window boundaries.
 * If the position exceeds the window boundaries, it wraps around to the opposite side.
 * @param position The position vector to wrap.
 * @param windowSize The size of the window (in pixels) represented by a vector.
 ***/
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

/***
 * Function to normalize a vector, making its magnitude equal to 1 while preserving its direction.
 * @param vector The vector to normalize.
 * @return The normalized vector.
 ***/
inline sf::Vector2f normalize(sf::Vector2f vector)
{
    sf::Vector2f normalisedVector;
    float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (magnitude != 0) {
        normalisedVector = vector / magnitude;
    }
    return normalisedVector;
}

/***
 * Function to calculate the magnitude (length) of a vector.
 * @param vector The vector whose magnitude is to be calculated.
 * @return The magnitude of the vector.
 ***/
inline float vectorMagnitude(sf::Vector2f vector)
{
    return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

/***
 * Function to calculate the distance between two vectors.
 * @param vectorA The first vector.
 * @param vectorB The second vector.
 * @return The distance between vectorA and vectorB.
 ***/
inline float vectorDistance(sf::Vector2f vectorA, sf::Vector2f vectorB)
{
    float deltaX = vectorB.x - vectorA.x;
    float deltaY = vectorB.y - vectorA.y;
    return std::sqrt(deltaX * deltaX + deltaY * deltaY);
}

/***
 * Function to calculate the dot product of two vectors.
 * @param vectorA The first vector.
 * @param vectorB The second vector.
 * @return The dot product of vectorA and vectorB.
 ***/
inline float vectorDotProduct(sf::Vector2f vectorA, sf::Vector2f vectorB)
{
    float dotProduct = vectorA.x * vectorB.x + vectorA.y * vectorB.y;

    return dotProduct;
}

/***
 * Function to cast a ray from a position in a specified direction and check for intersections with points.
 * @param position The starting position of the ray.
 * @param direction The direction in which the ray is cast.
 * @param length The length of the ray.
 * @param points A vector containing points to check for intersections with the ray.
 * @param hitPoint If an intersection is found, this will contain the coordinates of the hit point.
 * @return True if the ray intersects with any of the points, false otherwise.
 ***/
inline bool castRay(const sf::Vector2f& position, const sf::Vector2f& direction, float length, const std::vector<sf::Vector2f>& points, sf::Vector2f& hitPoint) {
    bool hasHit = false;
    float closestDistance = std::numeric_limits<float>::max();

    sf::Vector2f normalizedDirection = normalize(direction);

    for (const auto& point : points) {
        sf::Vector2f toPoint = point - position;
        float projectionLength = vectorDotProduct(toPoint, normalizedDirection);

        // Check if the point is within the ray's length and in the direction of the ray
        if (projectionLength > 0 && projectionLength <= length) {
            sf::Vector2f closestPointOnRay = position + normalizedDirection * projectionLength;
            float distanceToPoint = vectorMagnitude(point - closestPointOnRay);

            // Check if this point is the closest hit within the ray's length
            if (distanceToPoint < closestDistance) {
                closestDistance = distanceToPoint;
                hitPoint = closestPointOnRay;
                hasHit = true;
            }
        }
    }

    return hasHit;
}

/***
 * Function to rotate a 2D vector by a specified angle.
 * @param vec The vector to be rotated.
 * @param angle The angle (in degrees) by which to rotate the vector.
 * @return The rotated vector.
 ***/
inline sf::Vector2f rotate(const sf::Vector2f& vec, float angle) {
    float rad = angle * PI / 180.0f;
    return sf::Vector2f(
        vec.x * cos(rad) - vec.y * sin(rad),
        vec.x * sin(rad) + vec.y * cos(rad)
    );
}

/***
 * Function to check if a point intersects with a circle.
 * @param point The point to check for intersection.
 * @param circleCenter The center of the circle.
 * @param circleRadius The radius of the circle.
 * @return True if the point intersects with the circle, false otherwise.
 ***/
inline bool intersects(const sf::Vector2f& point, const sf::Vector2f& circleCenter, float circleRadius) {
    return vectorMagnitude(point - circleCenter) < circleRadius;
}