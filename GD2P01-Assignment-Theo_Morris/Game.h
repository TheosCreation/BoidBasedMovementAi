/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Game.h
Description : Declaration of the Game class, which manages the main game loop and controls game objects and user interface elements.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#pragma once

#include <iostream>
#include <vector>
#include "Agent.h"
#include "Obstacle.h"
#include "Button.h"

#include <SFML/Graphics.hpp>

class Game
{
private:
	sf::Vector2i gameWindowSize;
	sf::RenderWindow* gameWindow;

	sf::Vector2i uiWindowSize;
	sf::RenderWindow* uiWindow;
	sf::Event event;
	sf::Clock clock;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	sf::Font font;
	sf::Text debugText;

	float buttonWidth = 260.0f;
	float buttonHeight = 60.0f;
	float buttonSpacing = 15.0f;

	std::vector<std::unique_ptr<Button>> behaviourButtons;
	std::vector<std::unique_ptr<Button>> functionalButtons;

	//Game objects
	std::vector<std::unique_ptr<Agent>> agents;
	std::vector<std::unique_ptr<Obstacle>> obstacles;

	MovementBehavior currentSelectedBehaviour = MovementBehavior::Wander;

	void initWindow();
	void initUiWindow();
	void initGame();
	void initUi();
	void initObstacles();

public:
	Game();
	~Game();

	const bool isRunning() const;

	void spawnAgent(int spawnPositionX, int spawnPositionY, MovementBehavior agentMovementBehaviour);
	void spawnObstacle(float spawnPositionX, float spawnPositionY, float radius);

	void updateAgents(float deltaTime);
	void updateMousePositions(float deltaTime);

	void pollEvents();
	void update();
	void render();
	void reset();
};