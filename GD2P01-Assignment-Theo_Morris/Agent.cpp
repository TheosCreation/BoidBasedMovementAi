/***
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) 2024 Media Design School
File Name : Agent.cpp
Description : Implementation of the Agent class, which represents an entity capable of autonomous movement and behavior in a simulation environment.
Author : Theo Morris
Mail : theo.morris@mds.ac.nz
**/

#include "Agent.h"

Agent::Agent(int spawnPositionX, int spawnPositionY, Agent* agentToFollow = nullptr, MovementBehavior movementType = MovementBehavior::Wander) : m_agentToFollow(agentToFollow)
{
    loadSprite(movementType);
    // Set the texture, scale, origin, rotation position of the sprite
    m_sprite.setTexture(m_texture);

    m_sprite.setScale(0.1f, 0.1f);

    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);

    m_pos = sf::Vector2f(spawnPositionX, spawnPositionY);
    m_sprite.setPosition(m_pos);
    
    //calculate random direction to move towards
    wdelta = static_cast<float>(rand()) / RAND_MAX * 360.0f;

    m_rot = wdelta;
    m_sprite.setRotation(m_rot);

    m_velocity = sf::Vector2f(cos(m_rot) * INITIAL_SPEED, sin(m_rot) * INITIAL_SPEED);

    // Initialize weights based on movementType
    initializeWeights(movementType);
}

Agent::~Agent()
{
}

void Agent::initializeWeights(MovementBehavior movementType) {
    // Set specific weights based on the movement type
    switch (movementType) {
    case MovementBehavior::Seek:
        avoidanceWeight = 2.0f;
        seekWeight = 1.0f;
        break;
    case MovementBehavior::Flee:
        separationWeight = 1.0f;
        avoidanceWeight = 2.0f;
        fleeWeight = 1.0f;
        break;
    case MovementBehavior::Pursue:
        avoidanceWeight = 2.0f;
        pursuitWeight = 1.0f;
        break;
    case MovementBehavior::Evade:
        avoidanceWeight = 2.0f;
        evadeWeight = 1.0f;
        break;
    case MovementBehavior::Wander:
        avoidanceWeight = 2.0f;
        wanderWeight = 1.0f;
        break;
    case MovementBehavior::Arrival:
        avoidanceWeight = 2.0f;
        arrivalWeight = 1.0f;
        break;
    case MovementBehavior::Flocking:
        cohesionWeight = 1.0f;
        alignmentWeight = 1.0f;
        separationWeight = 2.0f;
        avoidanceWeight = 2.0f;
        break;
    case MovementBehavior::FollowLeader:
        separationWeight = 1.0f;
        avoidanceWeight = 2.0f;
        followingLeaderWeight = 1.0f;
        break;
    case MovementBehavior::Queue:
        avoidanceWeight = 2.0f;
        queueingWeight = 1.0f;
        break;
    default:
        break;
    }
}

void Agent::loadSprite(MovementBehavior movementType) {

    // Set specific weights based on the movement type
    switch (movementType) {
    case MovementBehavior::Seek:
        m_texture.loadFromFile("Assets/Textures/Seek.png");
        break;
    case MovementBehavior::Flee:
        m_texture.loadFromFile("Assets/Textures/Flee.png");
        break;
    case MovementBehavior::Pursue:
        m_texture.loadFromFile("Assets/Textures/Pursue.png");
        break;
    case MovementBehavior::Evade:
        m_texture.loadFromFile("Assets/Textures/Evade.png");
        break;
    case MovementBehavior::Wander:
        m_texture.loadFromFile("Assets/Textures/Wander.png");
        break;
    case MovementBehavior::Arrival:
        m_texture.loadFromFile("Assets/Textures/Arrival.png");
        break;
    case MovementBehavior::Flocking:
        m_texture.loadFromFile("Assets/Textures/Flocking.png");
        break;
    case MovementBehavior::FollowLeader:
        m_texture.loadFromFile("Assets/Textures/FollowLeader.png");
        break;
    case MovementBehavior::Queue:
        m_texture.loadFromFile("Assets/Textures/Queue.png");
        break;
    default:
        m_texture.loadFromFile("Assets/Textures/Wander.png");
        break;
    }
}

void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

void Agent::update(float deltaTime, const sf::Vector2u& windowSize, const std::vector<std::unique_ptr<Agent>>& agents, const std::vector<std::unique_ptr<Obstacle>>& obstacles, const sf::Vector2i& target)
{
    // Initialize forces and counters
    sf::Vector2f cohesionForce(0.0f, 0.0f);
    sf::Vector2f alignmentForce(0.0f, 0.0f);
    sf::Vector2f separationForce(0.0f, 0.0f);
    int cohesionCount = 0;
    int alignmentCount = 0;
    int separationCount = 0;

    // Iterate through other agents
    for (const auto& agent : agents) {
        if (agent.get() != this) {
            float distance = vectorDistance(getPosition(), agent->getPosition());

            if (distance < NEIGHBOR_RADIUS) {
                // Cohesion add position of nearby agents
                cohesionForce += agent->getPosition();
                cohesionCount++;

                // Alignment add velocity of nearby agents
                alignmentForce += agent->getVelocity();
                alignmentCount++;

                // Separation move away from nearby agents
                if (distance < SEPARATION_RADIUS) {
                    sf::Vector2f diff = getPosition() - agent->getPosition();
                    if (distance != 0) {
                        diff /= distance;
                    }
                    separationForce += diff;
                    separationCount++;
                }
            }
        }
    }

    //apply weights and calculate forces

    if (cohesionCount > 0) {
        // Calculate average position of nearby agents for cohesion
        cohesionForce = (cohesionForce / static_cast<float>(cohesionCount) - getPosition());
        cohesionForce = normalize(cohesionForce);
        cohesionForce *= deltaTime;
        cohesionForce *= alignmentWeight;
    }

    if (alignmentCount > 0) {
        // Calculate average velocity of nearby agents for alignment
        alignmentForce = (alignmentForce / static_cast<float>(alignmentCount) - getVelocity());
        alignmentForce = normalize(alignmentForce);
        alignmentForce *= deltaTime;
        alignmentForce *= alignmentWeight;
    }
    if (separationCount > 0) {
        // Calculate separation force
        separationForce = separationForce / static_cast<float>(separationCount);
        separationForce = normalize(separationForce);
        separationForce *= deltaTime;
        separationForce *= separationWeight;
    }

    // other behaviours
    sf::Vector2f wanderForce;
    sf::Vector2f seekForce;
    sf::Vector2f fleeForce;
    sf::Vector2f pursuitForce;
    sf::Vector2f evadeForce;
    sf::Vector2f arrivalForce;
    sf::Vector2f avoidanceForce;
    sf::Vector2f queueingForce;
    sf::Vector2f followingLeaderForce;

    // wander
    if (wanderWeight > 0)
    {
        wanderForce = wander(deltaTime);
        wanderForce *= wanderWeight;
    }

    // seek / flee
    if (seekWeight > 0)
    {
        seekForce = seek(sf::Vector2f(target), deltaTime);
        seekForce *= seekWeight;
    }
    if (fleeWeight > 0)
    {
        fleeForce = flee(sf::Vector2f(target), deltaTime);
        fleeForce *= fleeWeight;
    }

    // pursuit / evade
    // calculates future from the target
    sf::Vector2f targetDisplacement = sf::Vector2f(target - targetPreviousPos);

    targetPreviousPos = target;

    sf::Vector2f targetVelocity = targetDisplacement / deltaTime;

    if (pursuitWeight > 0)
    {
        pursuitForce = pursuit(sf::Vector2f(target), targetVelocity, deltaTime);
        pursuitForce *= pursuitWeight;
    }
    if (evadeWeight > 0)
    {
        evadeForce = evade(sf::Vector2f(target), targetVelocity, deltaTime);
        evadeForce *= evadeWeight;
    }

    // arrival
    if (arrivalWeight > 0)
    {
        arrivalForce = arrival(sf::Vector2f(target), deltaTime);
        arrivalForce *= arrivalWeight;
    }

    // obstacle avoidance
    if (avoidanceWeight > 0)
    {
        avoidanceForce = obstacleAvoidance(obstacles, deltaTime);
        avoidanceForce *= avoidanceWeight;
    }

    // queueing
    if (queueingWeight > 0)
    {
        queueingForce = queueing(deltaTime);
        queueingForce *= queueingWeight;
    }
    
    // following leader
    if (followingLeaderWeight > 0)
    {
        followingLeaderForce = followingLeader(deltaTime);
        followingLeaderForce *= followingLeaderWeight;
    }

    // Calculate total force
    sf::Vector2f totalForce = cohesionForce + alignmentForce + separationForce + seekForce + fleeForce + pursuitForce + evadeForce + wanderForce + arrivalForce + avoidanceForce + queueingForce + followingLeaderForce;

    if (vectorMagnitude(totalForce) > MAX_FORCE) {
        totalForce = normalize(totalForce) * MAX_FORCE;
    }

    // Update velocity
    m_velocity += totalForce;
    if (vectorMagnitude(m_velocity) > MAX_SPEED) {
        m_velocity = normalize(m_velocity) * MAX_SPEED;
    }

    // Update position and wraps the position around the screen boarders
    m_pos += m_velocity;
    wrapPosition(m_pos, windowSize);

    // Update rotation
    m_rot = std::atan2(m_velocity.y, m_velocity.x) * 180.0f / PI;
    m_sprite.setRotation(m_rot);

    // Update sprite position
    m_sprite.setPosition(m_pos);
}

sf::Vector2f Agent::seek(const sf::Vector2f& target, float dt)
{
    // Calculate desired velocity
    sf::Vector2f desiredVelocity = target - m_pos;
    float distance = vectorMagnitude(desiredVelocity);

    // Check if the distance is greater than zero to avoid division by zero
    if (distance > 0) {
        desiredVelocity = normalize(desiredVelocity);

        // Scale the desired velocity to the maximum speed
        desiredVelocity *= MAX_SPEED;

        // Calculate steering force
        sf::Vector2f steering = (desiredVelocity - m_velocity);

        // Normalize the steering force and scale it to the maximum force
        steering = normalize(steering);
        steering *= MAX_FORCE;

        // Apply dt to scale the steering force according to the frame time
        steering *= dt;

        return steering;
    }
    else {
        // If the agent is already at the target position, return zero steering force
        return sf::Vector2f(0.0f, 0.0f);
    }
}

sf::Vector2f Agent::flee(const sf::Vector2f& target, float dt)
{
    // Fleeing is essentially seeking the target in the opposite direction
    return seek(target, dt) * -1.0f;
}

sf::Vector2f Agent::pursuit(const sf::Vector2f& targetPos, const sf::Vector2f& targetVel, float dt)
{
    // Calculate the future position of the target based on its velocity
    sf::Vector2f futurePosition = targetPos + (targetVel * PREDICTION_TIME);

    return seek(futurePosition, dt);
}

sf::Vector2f Agent::evade(const sf::Vector2f& targetPos, const sf::Vector2f& targetVel, float dt)
{
    // Evading is essentially pursuing the predicted future position of the target in the opposite direction
    return pursuit(targetPos, targetVel, dt) * -1.0f;
}

sf::Vector2f Agent::wander(float dt)
{
    // Normalize the velocity to find the forward direction
    sf::Vector2f direction = normalize(m_velocity);

    // Calculate the center of the circle in front of the agent
    sf::Vector2f center = m_pos + direction;

    wdelta += static_cast<float>(rand()) / RAND_MAX * 0.25 * WANDERNOICE - 0.125 * WANDERNOICE;

    // Calculate the offset from the center using the random angle
    float x = std::cos(wdelta);
    float y = std::sin(wdelta);
    sf::Vector2f offset(x, y);

    // Calculate the new target position
    sf::Vector2f target = center + offset;

    // Seek towards the new target
    return seek(target, dt);
}

sf::Vector2f Agent::arrival(const sf::Vector2f& target, float dt)
{
    // Calculate desired velocity
    sf::Vector2f desiredVelocity = target - m_pos;
    float distance = vectorMagnitude(desiredVelocity);

    // Check if the distance is greater than zero to avoid division by zero
    if (distance > 0) {
        desiredVelocity = normalize(desiredVelocity);

        if (distance < ARRIVAL_RADIUS)
        {
            desiredVelocity *= MAX_SPEED * (distance / ARRIVAL_RADIUS);
        }
        else
        {
            desiredVelocity *= MAX_SPEED;
        }

        // Calculate steering force
        sf::Vector2f steering = desiredVelocity - m_velocity;

        // Normalize the steering force and scale it to the maximum force
        steering = normalize(steering);
        steering *= MAX_FORCE;

        // Apply dt to scale the steering force according to the frame time
        steering *= dt;

        return steering;
    }
    else {
        // If the agent is already at the target position, return zero steering force
        return sf::Vector2f(0.0f, 0.0f);
    }
}

sf::Vector2f Agent::obstacleAvoidance(const std::vector<std::unique_ptr<Obstacle>>& obstacles, float dt)
{
    sf::Vector2f avoidanceForce(0.0f, 0.0f);
    int count = 0;

    for (const auto& obstacle : obstacles) {
        sf::Vector2f toObstacle = obstacle->getPosition() - m_pos;
        float distance = vectorMagnitude(toObstacle);

        // Check if the obstacle is in the path of the agent
        if (distance < obstacle->getRadius() + AVOIDANCE_DISTANCE) {
            // Calculate a force to steer away from the obstacle
            sf::Vector2f steerAway = normalize(toObstacle) * -1.0f;
            avoidanceForce += steerAway;
            count++;
        }
    }

    if (count > 0) {
        avoidanceForce /= static_cast<float>(count);
        avoidanceForce = normalize(avoidanceForce) * MAX_FORCE;
    }

    return avoidanceForce * dt;
}

sf::Vector2f Agent::followingLeader(float dt)
{
    sf::Vector2f followingForce(0.0f, 0.0f);

    if (m_agentToFollow != nullptr) {
        // Calculate the behind point from the leader
        sf::Vector2f toLeader = m_agentToFollow->getPosition() - this->getPosition();
        sf::Vector2f behindPoint = m_agentToFollow->getPosition() - normalize(m_agentToFollow->getVelocity()) * LEADER_BEHIND_DIST;

        // Use the arrival function to move towards the behind point
        followingForce += arrival(behindPoint, dt);
    }
    else
    {
        // If there is no leader to follow means this is the leader and wander
        followingForce = wander(dt);
    }

    return followingForce;
}

sf::Vector2f Agent::queueing(float dt)
{
    sf::Vector2f queueingForce(0.0f, 0.0f);

    if (m_agentToFollow != nullptr) {
        sf::Vector2f frontAgentPos = m_agentToFollow->getPosition();
        sf::Vector2f toFrontAgent = frontAgentPos - m_pos;
        float distance = vectorMagnitude(toFrontAgent);

        if (distance > QUEUE_DISTANCE) {
            queueingForce = seek(frontAgentPos, dt);
        }
    }
    else
    {
        // If there is no leader to follow means this is the leader and wander
        queueingForce = wander(dt);
    }
    return queueingForce;
}

sf::Vector2f Agent::wallFollowing(const std::vector<std::unique_ptr<Obstacle>>& obstacles, float dt) {
    // Extract center points from obstacles
    std::vector<sf::Vector2f> points;
    for (const auto& obstacle : obstacles) {
        points.push_back(obstacle->getPosition());
    }

    // Cast rays to the left and right
    sf::Vector2f leftRayDirection = sf::Vector2f(-m_velocity.y, m_velocity.x); // Perpendicular to velocity
    sf::Vector2f rightRayDirection = sf::Vector2f(m_velocity.y, -m_velocity.x); // Perpendicular to velocity

    sf::Vector2f hitPoint;
    sf::Vector2f targetPoint;
    bool targetFound = false;

    // Check for walls on the left
    if (castRay(m_pos, leftRayDirection, DETECTION_RAY_LENGTH, points, hitPoint)) {
        float distance = vectorDistance(m_pos, hitPoint);
        if (distance < DESIRED_DISTANCE_FROM_WALL) {
            // Calculate target point to steer right
            targetPoint = hitPoint + normalize(rightRayDirection) * DESIRED_DISTANCE_FROM_WALL;
            targetFound = true;
        }
    }

    // Check for walls on the right
    if (!targetFound && castRay(m_pos, rightRayDirection, DETECTION_RAY_LENGTH, points, hitPoint)) {
        float distance = vectorDistance(m_pos, hitPoint);
        if (distance < DESIRED_DISTANCE_FROM_WALL) {
            // Calculate target point to steer left
            targetPoint = hitPoint + normalize(leftRayDirection) * DESIRED_DISTANCE_FROM_WALL;
            targetFound = true;
        }
    }

    // If a target point is found, use the seek function to move towards it
    if (targetFound) {
        return seek(targetPoint, dt);
    }

    return sf::Vector2f(0.0f, 0.0f);
}