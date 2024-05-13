/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Game.cpp
Description : Implementation of the Game class, which manages the main game loop and controls game objects and user interface elements.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#include "Game.h"
#include <sstream>

void Game::initWindow()
{
	gameWindowSize = sf::Vector2i(1000, 1000);
	gameWindow = new sf::RenderWindow(sf::VideoMode(gameWindowSize.x, gameWindowSize.y), "Game Window", sf::Style::Titlebar | sf::Style::Close);
}

void Game::initUiWindow()
{
	uiWindowSize = sf::Vector2i(300, 1000);
	uiWindow = new sf::RenderWindow(sf::VideoMode(uiWindowSize.x, uiWindowSize.y), "Selector Window", sf::Style::Titlebar | sf::Style::Close);

	sf::Vector2i gameWindowPosition = gameWindow->getPosition();
	sf::Vector2i uiWindowPosition(gameWindowPosition.x - (uiWindowSize.x + 10.0f), gameWindowPosition.y);
	uiWindow->setPosition(uiWindowPosition);
}

void Game::initGame()
{
	gameWindow = nullptr;
	uiWindow = nullptr;
}

void Game::initUi()
{
	font.loadFromFile("Assets/Fonts/OpenSans-Regular.ttf");

	debugText.setCharacterSize(16);
	debugText.setFillColor(sf::Color::Green);
	debugText.setFont(font);
	debugText.setPosition(20.0f, 20.0f);

	// Calculate the x position for left alignment
	float xPosition = 20.0f;

	// Initialize buttons
	std::unique_ptr<Button> seekButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Seek");
	seekButton->setPosition(sf::Vector2f(xPosition, 50.f));
	behaviourButtons.push_back(std::move(seekButton));

	std::unique_ptr<Button> fleeButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Flee");
	fleeButton->setPosition(sf::Vector2f(xPosition, 50.f + buttonHeight + buttonSpacing));
	behaviourButtons.push_back(std::move(fleeButton));

	std::unique_ptr<Button> pursueButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Pursue");
	pursueButton->setPosition(sf::Vector2f(xPosition, 50.f + 2 * (buttonHeight + buttonSpacing)));
	behaviourButtons.push_back(std::move(pursueButton));

	std::unique_ptr<Button> evadeButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Evade");
	evadeButton->setPosition(sf::Vector2f(xPosition, 50.f + 3 * (buttonHeight + buttonSpacing)));
	behaviourButtons.push_back(std::move(evadeButton));

	std::unique_ptr<Button> wanderButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Wander");
	wanderButton->setPosition(sf::Vector2f(xPosition, 50.f + 4 * (buttonHeight + buttonSpacing)));
	behaviourButtons.push_back(std::move(wanderButton));
	
	std::unique_ptr<Button> arrivalButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Arrival");
	arrivalButton->setPosition(sf::Vector2f(xPosition, 50.f + 5 * (buttonHeight + buttonSpacing)));
	behaviourButtons.push_back(std::move(arrivalButton));
	
	std::unique_ptr<Button> flockingButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Flocking");
	flockingButton->setPosition(sf::Vector2f(xPosition, 50.f + 6 * (buttonHeight + buttonSpacing)));
	behaviourButtons.push_back(std::move(flockingButton));
	
	std::unique_ptr<Button> followerButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "FollowLeader");
	followerButton->setPosition(sf::Vector2f(xPosition, 50.f + 7 * (buttonHeight + buttonSpacing)));
	behaviourButtons.push_back(std::move(followerButton));
	
	std::unique_ptr<Button> queueingButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Queueing");
	queueingButton->setPosition(sf::Vector2f(xPosition, 50.f + 8 * (buttonHeight + buttonSpacing)));
	behaviourButtons.push_back(std::move(queueingButton));

	std::unique_ptr<Button> resetButton = std::make_unique<Button>(sf::Vector2f(buttonWidth, buttonHeight), font, "Reset");
	resetButton->setPosition(sf::Vector2f(xPosition, 50.f + 11 * (buttonHeight + buttonSpacing)));
	functionalButtons.push_back(std::move(resetButton));
}

void Game::initObstacles()
{
	spawnObstacle(150.0f, 200.0f, 50.0f);
	spawnObstacle(800.0f, 150.0f, 50.0f);
	spawnObstacle(100.0f, 900.0f, 50.0f);
	spawnObstacle(500.0f, 300.0f, 60.0f);
	spawnObstacle(700.0f, 700.0f, 65.0f);
	spawnObstacle(300.0f, 100.0f, 55.0f);
	spawnObstacle(250.0f, 750.0f, 65.0f);
}

Game::Game()
{
	initGame();
	initWindow();
	initUiWindow();
	initUi();
	initObstacles();
}

Game::~Game()
{
	delete gameWindow;
	delete uiWindow;
}

const bool Game::isRunning() const
{
	return gameWindow->isOpen();
}

void Game::spawnAgent(int spawnPositionX, int spawnPositionY, MovementBehavior agentMovementBehaviour)
{
	// Create a new Agent object dynamically
	std::unique_ptr<Agent> newAgent;

	// Check if the agents vector is empty
	if (agents.empty()) {
		// If it's empty, pass nullptr as there is no agent to follow or be in front
		newAgent = std::make_unique<Agent>(spawnPositionX, spawnPositionY, nullptr, agentMovementBehaviour);
	}
	else {
		// If not empty, decide based on the movement behavior
		if (agentMovementBehaviour == MovementBehavior::FollowLeader) {
			// Follow the first agent if the behavior is FollowLeader
			newAgent = std::make_unique<Agent>(spawnPositionX, spawnPositionY, agents.front().get(), agentMovementBehaviour);
		}
		else {
			// Otherwise, follow the last agent
			newAgent = std::make_unique<Agent>(spawnPositionX, spawnPositionY, agents.back().get(), agentMovementBehaviour);
		}
	}

	// Store the smart pointer to the new Agent object in the agents vector
	agents.push_back(std::move(newAgent));

	std::cout << "Agent spawned at location: " << spawnPositionX << ", " << spawnPositionY << std::endl;
}

void Game::spawnObstacle(float spawnPositionX, float spawnPositionY, float radius)
{
	// Create a new Obstacle object dynamically
	std::unique_ptr<Obstacle> newObstacle = std::make_unique<Obstacle>(sf::Vector2f(spawnPositionX, spawnPositionY), radius);

	// Store the smart pointer to the new Obstacle object in the obstacles vector
	obstacles.push_back(std::move(newObstacle));

	std::cout << "Obstacle spawned at location: " << spawnPositionX << ", " << spawnPositionY << std::endl;
}

void Game::pollEvents()
{
	//Game Window
	while (gameWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			gameWindow->close();
			uiWindow->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				gameWindow->close();
				uiWindow->close();
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawnAgent(mousePosWindow.x, mousePosWindow.y, currentSelectedBehaviour);
			}
			break;
		}
	}

	//UI Window
	while (uiWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			gameWindow->close();
			uiWindow->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				gameWindow->close();
				uiWindow->close();
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				for (int i = 0; i < behaviourButtons.size(); ++i) {
					if (behaviourButtons[i]->isMouseOver(*uiWindow)) {
						currentSelectedBehaviour = static_cast<MovementBehavior>(i);
						return;
					}
				}
				for (int i = 0; i < functionalButtons.size(); ++i) {
					if (functionalButtons[i]->isMouseOver(*uiWindow)) {
						reset();
						return;
					}
				}
			}
			break;
		}
	}
}

void::Game::updateAgents(float deltaTime)
{
	for (auto& agentPtr : agents) {
		agentPtr->update(deltaTime, gameWindow->getSize(), agents, obstacles, mousePosWindow);
	}
}

void::Game::updateMousePositions(float deltaTime)
{
	mousePosScreen = sf::Mouse::getPosition();
	mousePosWindow = sf::Mouse::getPosition(*gameWindow);
	mousePosView = gameWindow->mapPixelToCoords(mousePosWindow);
}

void Game::update()
{
	pollEvents();

	float dt = clock.restart().asSeconds();

	updateMousePositions(dt);

	updateAgents(dt);

	std::stringstream ss;
	ss << "Screen: " << mousePosScreen.x << " " << mousePosScreen.y << "\n"
		<< "Window: " << mousePosWindow.x << " " << mousePosWindow.y << "\n"
		<< "View: " << mousePosView.x << " " << mousePosView.y << "\n"
		<< "Agents: " << agents.size() << "\n";

	debugText.setString(ss.str());
}

void Game::render()
{
	//Game Window
	gameWindow->clear(sf::Color::Black);

	//Draw Game Objects
	for (const auto& agentPtr : agents)
	{
		gameWindow->draw(*agentPtr);
	}
	
	for (const auto& obstaclePtr : obstacles)
	{
		gameWindow->draw(*obstaclePtr);
	}

	//Draw Ui
	gameWindow->draw(debugText);

	gameWindow->display();

	//UI Window
	uiWindow->clear(sf::Color::Black);
	for (const auto& button : behaviourButtons) {
		uiWindow->draw(*button);
	}
	for (const auto& button : functionalButtons) {
		uiWindow->draw(*button);
	}
	uiWindow->display();
}

void Game::reset()
{
	agents.clear();
	std::cout << "Game has been reset. All agents have been cleared." << std::endl;
}