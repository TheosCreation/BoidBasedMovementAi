/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Game.h
Description :
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#pragma once

#include <iostream>
#include <vector>
#include "Agent.h"

#include <SFML/Graphics.hpp>

class Game
{
private:
	sf::Vector2i windowSize;
	sf::RenderWindow* window;
	sf::Event event;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	sf::Font font;
	sf::Text debugText;

	//Game objects
	std::vector<std::unique_ptr<Agent>> agents;

	void initWindow();
	void initGame();
	void initUi();
	void initAgents();
public:
	Game();
	~Game();

	const bool isRunning() const;

	void spawnAgent(int spawnPositionX, int spawnPositionY);

	void updateEnemies();
	void updateMousePositions();

	void pollEvents();
	void update();
	void render();
};