/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Button.cpp
Description : Implementation of the Button class, which represents a graphical user interface (GUI) button in a SFML-based application.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#include "Button.h"

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Draw the shape of the button
    target.draw(shape, states);

    // Draw the text on the button
    target.draw(text, states);
}