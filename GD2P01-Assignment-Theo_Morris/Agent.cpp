#include "Agent.h"

Agent::Agent(int spawnPositionX, int spawnPositionY)
{
    m_texture.loadFromFile("Assets/Textures/Wander.png");
    // Set texture for the sprite
    m_sprite.setTexture(m_texture);

    // Set the scale of the sprite to be accurate for the world
    m_sprite.setScale(0.1f, 0.1f);

    // Set position and size of the sprite
    m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);

    // Set position of the sprite so that its center aligns with (spawnPositionX, spawnPositionY)
    m_pos = sf::Vector2f(spawnPositionX, spawnPositionY);
    m_sprite.setPosition(m_pos);

    //m_rot = distribution(mersenne);
    //m_sprite.setRotation(m_rot); 
    
    // Set a random velocity
    //float direction = m_rot * PI / 180.0f; // Convert rotation to radians
    //float speed = 2.0f; // Adjust speed as needed
    //m_velocity.x = std::cos(direction) * speed;
    //m_velocity.y = std::sin(direction) * speed;
}

Agent::~Agent()
{
}

void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

void Agent::update(float deltaTime, const sf::Vector2u& windowSize, const std::vector<std::unique_ptr<Agent>>& agents, const sf::Vector2i& target)
{
    // Update wander cooldown timer
    wanderTick += deltaTime;
    updateTick += deltaTime;

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
            float distance = vectorDistance(agent->getPosition(), getPosition());

            if (distance < NEIGHBOR_RADIUS) {
                // Cohesion: Add position of nearby agents
                cohesionForce += agent->getPosition();
                cohesionCount++;

                // Alignment: Add velocity of nearby agents
                alignmentForce += agent->getVelocity();
                alignmentCount++;

                // Separation: Move away from nearby agents
                if (distance < SEPARATION_RADIUS) {
                    separationForce += getPosition() - agent->getPosition();
                    separationCount++;
                }
            }
        }
    }

    // Apply weights
    float cohesionWeight = 1.0f;
    float alignmentWeight = 1.0f;
    float separationWeight = 1.0f;

    //if clicking then activate seek
    float seekWeight = 0.0f;
    float fleeWeight = 1.0f;
    float wanderWeight = 1.0f;

    ///////////////////////////////////////////////////////////

    //if pursue behaviour 
    sf::Vector2f targetDisplacement = sf::Vector2f(target - targetPreviousPos);

    targetPreviousPos = target;
    
    sf::Vector2f targetVelocity = targetDisplacement / deltaTime;
    //////////////////////////////////////////////////////////////////////////////
    sf::Vector2f wanderForce;
    if (wanderTick > WANDERCOOLDOWNDURATION)
    {
        wanderForce = wander();
        wanderTick = 0.0f;
    }
    wanderForce *= wanderWeight;
    
    sf::Vector2f seekForce = seek(sf::Vector2f(target));
    seekForce *= seekWeight;

    if (cohesionCount > 0) {
        // Calculate average position of nearby agents for cohesion
        cohesionForce /= static_cast<float>(cohesionCount);

        // Apply movement behaviour towards the mouse position
        cohesionForce *= cohesionWeight;
    }

    if (alignmentCount > 0) {
        // Calculate average velocity of nearby agents for alignment
        alignmentForce /= static_cast<float>(alignmentCount);
        alignmentForce *= alignmentWeight;
    }

    if (separationCount > 0) {
        // Calculate separation force
        separationForce /= static_cast<float>(separationCount);

        // Apply movement behaviour towards the mouse position

        separationForce *= separationWeight;
    }

    // Calculate total force
    sf::Vector2f totalForce = cohesionForce + alignmentForce + separationForce + wanderForce + seekForce;

    if (vectorMagnitude(totalForce) > MAX_FORCE) {
        totalForce = normalize(totalForce) * MAX_FORCE;
    }

    sf::Vector2f acceleration = totalForce / MASS;
    m_velocity += acceleration * deltaTime;

    // Update velocity
    m_velocity += totalForce;
    if (vectorMagnitude(m_velocity) > MAX_SPEED) {
        m_velocity = normalize(m_velocity) * MAX_SPEED;
    }

    // Update position
    m_pos += m_velocity;
    wrapPosition(m_pos, windowSize);

    // Update rotation
    m_rot = std::atan2(m_velocity.y, m_velocity.x) * 180.0f / PI;
    m_sprite.setRotation(m_rot);

    // Update sprite position
    m_sprite.setPosition(m_pos);
}

sf::Vector2f Agent::seek(const sf::Vector2f& target)
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
        sf::Vector2f steering = desiredVelocity - m_velocity;

        // Normalize the steering force and scale it to the maximum force
        steering = normalize(steering);
        steering *= MAX_FORCE;

        return steering;
    }
    else {
        // If the agent is already at the target position, return zero steering force
        return sf::Vector2f(0.0f, 0.0f);
    }
}

sf::Vector2f Agent::flee(const sf::Vector2f& target)
{
    // Calculate desired velocity
    sf::Vector2f desiredVelocity = m_pos - target;
    float distance = vectorMagnitude(desiredVelocity);

    // Check if the distance is greater than zero to avoid division by zero
    if (distance > 0) {
        desiredVelocity = normalize(desiredVelocity);

        // Scale the desired velocity to the maximum speed
        desiredVelocity *= MAX_SPEED;

        // Calculate steering force
        sf::Vector2f steering = desiredVelocity - m_velocity;

        // Normalize the steering force and scale it to the maximum force
        steering = normalize(steering);
        steering *= MAX_FORCE;

        return steering;
    }
    else {
        // If the agent is already at the target position, return zero steering force
        return sf::Vector2f(0.0f, 0.0f);
    }
}

sf::Vector2f Agent::pursue(const sf::Vector2f& targetPos, const sf::Vector2f& targetVel)
{
    // Calculate the future position of the target based on its velocity
    sf::Vector2f futurePosition = targetPos + targetVel * PREDICTION_TIME;

    // Call the seek function with the future position of the target
    return seek(futurePosition);
}

sf::Vector2f Agent::evade(const sf::Vector2f& targetPos, const sf::Vector2f& targetVel)
{
    // Calculate the future position of the target based on its velocity
    sf::Vector2f futurePosition = targetPos + targetVel * PREDICTION_TIME;

    // Call the flee function with the future position of the target
    return flee(futurePosition);
}

sf::Vector2f Agent::wander()
{
    // Find center of circle
    sf::Vector2f direction = normalize(m_velocity);
    sf::Vector2f center = m_pos + direction * WANDERLENGTH;

    // Random walk
    float randomAngle = distribution(mersenne);
    m_rot += randomAngle;
    float x = WANDERRAD * std::cos(m_rot);
    float y = WANDERRAD * std::sin(m_rot);
    sf::Vector2f offset(x, y);
    sf::Vector2f target = center + offset;

    return seek(target);
}

sf::Vector2f Agent::arrival(const sf::Vector2f& target)
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

        return steering;
    }
    else {
        // If the agent is already at the target position, return zero steering force
        return sf::Vector2f(0.0f, 0.0f);
    }
}