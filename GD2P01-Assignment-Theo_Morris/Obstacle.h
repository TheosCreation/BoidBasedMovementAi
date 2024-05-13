/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Obstacle.h
Description : Defines the Obstacle class, which represents an obstacle in the game.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

class Obstacle : public sf::Drawable
{
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_pos;
	float m_radius;
public:
	Obstacle(sf::Vector2f position, float radius);
	~Obstacle();

	sf::Vector2f getPosition() const {
		return m_pos;
	}

	float getRadius() const {
		return m_radius;
	}

	// Override the draw function to make the agent drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};