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

    m_rot = distribution(mersenne);
    m_sprite.setRotation(m_rot); 
    
    // Set a random velocity
    float direction = m_rot * PI / 180.0f; // Convert rotation to radians
    float speed = 2.0f; // Adjust speed as needed
    m_velocity.x = std::cos(direction) * speed;
    m_velocity.y = std::sin(direction) * speed;
}

Agent::~Agent()
{
}

void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

void Agent::update(const sf::Vector2u& windowSize, const std::vector<std::unique_ptr<Agent>>& agents)
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

    if (cohesionCount > 0) {
        // Calculate average position of nearby agents for cohesion
        cohesionForce /= static_cast<float>(cohesionCount);
        cohesionForce = seek(cohesionForce) * cohesionWeight;
    }

    if (alignmentCount > 0) {
        // Calculate average velocity of nearby agents for alignment
        alignmentForce /= static_cast<float>(alignmentCount);
        normalize(alignmentForce);
        alignmentForce *= MAX_SPEED;
        alignmentForce *= alignmentWeight;
    }

    if (separationCount > 0) {
        // Calculate separation force
        separationForce /= static_cast<float>(separationCount);
        separationForce = seek(getPosition() + separationForce) * separationWeight;
    }

    // Calculate total force
    sf::Vector2f totalForce = cohesionForce + alignmentForce + separationForce;
    if (vectorMagnitude(totalForce) > MAX_FORCE) {
        normalize(totalForce);
        totalForce *= MAX_FORCE;
    }

    // Update velocity
    m_velocity += totalForce;
    if (vectorMagnitude(m_velocity) > MAX_SPEED) {
        normalize(m_velocity);
        m_velocity *= MAX_SPEED;
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
    float distanceSquared = desiredVelocity.x * desiredVelocity.x + desiredVelocity.y * desiredVelocity.y;

    // Check if the distance squared is greater than zero to avoid division by zero
    if (distanceSquared > 0) {
        // Calculate the distance and normalize the desired velocity
        float distance = std::sqrt(distanceSquared);
        desiredVelocity /= distance;

        // Scale the desired velocity to the maximum speed
        desiredVelocity *= MAX_SPEED;

        // Calculate steering force
        sf::Vector2f steering = desiredVelocity - m_velocity;

        // Normalize the steering force and scale it to the maximum force
        normalize(steering);
        steering *= MAX_FORCE;

        return steering;
    }
    else {
        // If the agent is already at the target position, return zero steering force
        return sf::Vector2f(0.0f, 0.0f);
    }
}