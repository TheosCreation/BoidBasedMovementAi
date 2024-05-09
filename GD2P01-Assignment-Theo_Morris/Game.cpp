/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Game.cpp
Description : 
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#include "Game.h"
#include <sstream>

void Game::initWindow()
{
	windowSize = sf::Vector2i(800, 600);
	window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "window");
}

void Game::initGame()
{
	window = nullptr;
}

void Game::initUi()
{
	font.loadFromFile("Assets/Fonts/OpenSans-Regular.ttf");

	debugText.setCharacterSize(16);
	debugText.setFillColor(sf::Color::Green);
	debugText.setFont(font);
	debugText.setPosition(20.0f, 20.0f);
}

void Game::initAgents()
{
	//spawnAgent(100, 100);
}

Game::Game()
{
	initGame();
	initWindow();
	initUi();
	initAgents();
}

Game::~Game()
{
	delete window;
}

const bool Game::isRunning() const
{
	return window->isOpen();
}

void Game::spawnAgent(int spawnPositionX, int spawnPositionY)
{
	// Create a new Agent object dynamically
	std::unique_ptr<Agent> newAgent = std::make_unique<Agent>(spawnPositionX, spawnPositionY);

	// Store the smart pointer to the new Agent object in the agents vector
	agents.push_back(std::move(newAgent));

	std::cout << "Agent spawned at location: " << spawnPositionX << ", " << spawnPositionY << std::endl;
}

void Game::pollEvents()
{
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				window->close();
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawnAgent(mousePosWindow.x, mousePosWindow.y);
			}
			break;
		}
	}
}

void::Game::updateEnemies()
{
}

void::Game::updateMousePositions()
{
	mousePosScreen = sf::Mouse::getPosition();
	mousePosWindow = sf::Mouse::getPosition(*window);
	mousePosView = window->mapPixelToCoords(mousePosWindow);
}

void Game::update()
{
	pollEvents();

	updateMousePositions();

	updateEnemies();

	std::stringstream ss;
	ss << "Screen: " << mousePosScreen.x << " " << mousePosScreen.y << "\n"
		<< "Window: " << mousePosWindow.x << " " << mousePosWindow.y << "\n"
		<< "View: " << mousePosView.x << " " << mousePosView.y << "\n";

	debugText.setString(ss.str());
}

void Game::render()
{
	window->clear(sf::Color::Black);

	//Draw Game Objects
	for (const auto& agentPtr : agents)
	{
		window->draw(*agentPtr);
	}

	//Draw Ui
	window->draw(debugText);

	window->display();
}
