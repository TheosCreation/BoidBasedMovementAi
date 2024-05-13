/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : main.cpp
Description : Entry point for the program. It initializes the game and runs the game loop.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#include <iostream>

#include "Game.h"

int main() 
{
    Game game;

    while (game.isRunning()) {
        //Update
        game.update();

        //Render
        game.render();
    }
}