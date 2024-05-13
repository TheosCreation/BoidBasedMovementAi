/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Button.h
Description : Declaration of the Button class, which represents a graphical user interface (GUI) button in a SFML-based application.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#pragma once

#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
public:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font& font;

    Button(sf::Vector2f size, sf::Font& font, const std::string& str)
        : font(font) {
        shape.setSize(size);
        shape.setFillColor(sf::Color::Blue);

        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(shape.getPosition() + sf::Vector2f(10.f, 5.f));
    }

    // Override the draw function to make the agent drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void setPosition(sf::Vector2f pos) {
        shape.setPosition(pos);
        text.setPosition(pos + sf::Vector2f(10.f, 5.f));
    }

    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }
};