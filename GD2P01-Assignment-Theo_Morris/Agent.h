/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Agent.h
Description : Declaration of the Agent class, which represents an entity capable of autonomous movement and behavior in a simulation environment.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#pragma once

#include <iostream>
#include <random>
#include "Math.h"
#include "Obstacle.h"

#include <SFML/Graphics.hpp>

const float NEIGHBOR_RADIUS = 500.0f;
const float SEPARATION_RADIUS = 50.0f;

const float INITIAL_SPEED = 0.1f;
const float MAX_SPEED = 0.1f;
const float MAX_FORCE = 1.0f;

const float PREDICTION_TIME = 100.0f;

const float WANDERNOICE = 0.1f;

const float AVOIDANCE_DISTANCE = 20.0f;

const float QUEUE_DISTANCE = 50.0f;

const float ARRIVAL_RADIUS = 300.0f;

const float LEADER_BEHIND_DIST = 50.0f;

const float DETECTION_RAY_LENGTH = 100.0f;
const float DESIRED_DISTANCE_FROM_WALL = 40.0f;

enum class MovementBehavior {
    Seek,
    Flee,
    Pursue,
    Evade,
    Wander,
    Arrival,
    Flocking,
    FollowLeader,
    Queue
};

class Agent : public sf::Drawable
{
private:
    Agent* m_agentToFollow;

    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Vector2f m_pos;
    sf::Vector2f m_velocity;
    float m_rot;
    sf::Vector2i targetPreviousPos;

    float wdelta;

    // init weights
    float cohesionWeight = 0.0f;
    float alignmentWeight = 0.0f;
    float separationWeight = 0.0f;
    float seekWeight = 0.0f;
    float fleeWeight = 0.0f;
    float pursuitWeight = 0.0f;
    float evadeWeight = 0.0f;
    float wanderWeight = 0.0f;
    float arrivalWeight = 0.0f;
    float avoidanceWeight = 0.0f;
    float queueingWeight = 0.0f;
    float followingLeaderWeight = 0.0f;
    float pathFollowingWeight = 0.0f;
    float crowdPathFollowingWeight = 0.0f;
    float wallFollowingWeight = 0.0f;

public:
    Agent(int spawnPositionX, int spawnPositionY, Agent* agentInFront, MovementBehavior movementType);
    ~Agent();

    void initializeWeights(MovementBehavior movementType);
    void loadSprite(MovementBehavior movementType);

    sf::Vector2f getPosition() const { return m_pos; }
    sf::Vector2f getVelocity() const { return m_velocity; }

    // Override the draw function to make the agent drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // update function to update all the forces and positions of the agent
    void update(float deltaTime, const sf::Vector2u& windowSize, const std::vector<std::unique_ptr<Agent>>& agents, const std::vector<std::unique_ptr<Obstacle>>& obstacles, const sf::Vector2i& target);

    // seek/flee
    sf::Vector2f seek(const sf::Vector2f& target, float dt);
    sf::Vector2f flee(const sf::Vector2f& target, float dt);

    // pursue/evade
    sf::Vector2f pursuit(const sf::Vector2f& targetPos, const sf::Vector2f& targetVel, float dt);
    sf::Vector2f evade(const sf::Vector2f& targetPos, const sf::Vector2f& targetVel, float dt);

    // wander
    sf::Vector2f wander(float dt);

    // arrival
    sf::Vector2f arrival(const sf::Vector2f& target, float dt);

    // obstacle avoidance
    sf::Vector2f obstacleAvoidance(const std::vector<std::unique_ptr<Obstacle>>& obstacles, float dt);

    // queueing
    sf::Vector2f queueing(float dt);

    // follower
    sf::Vector2f followingLeader(float dt);

    // wall following
    sf::Vector2f wallFollowing(const std::vector<std::unique_ptr<Obstacle>>& obstacles, float dt);
};