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
    // Implement flocking behaviors
    sf::Vector2f cohesion = sf::Vector2f(0.0f, 0.0f); // Initialize cohesion force
    sf::Vector2f alignment = sf::Vector2f(0.0f, 0.0f); // Initialize alignment force
    sf::Vector2f separation = sf::Vector2f(0.0f, 0.0f); // Initialize separation force

    int cohesionCount = 0;
    int alignmentCount = 0;
    int separationCount = 0;

    for (const auto& agent : agents) {
        float distance = std::sqrt(std::pow(agent->m_pos.x - m_pos.x, 2) + std::pow(agent->m_pos.y - m_pos.y, 2));

        if (agent.get() != this && distance < NEIGHBOR_RADIUS) {
            // Cohesion: Add position of nearby agents
            cohesion += agent->getPosition();
            cohesionCount++;

            // Alignment: Add velocity of nearby agents
            alignment += agent->getVelocity();
            alignmentCount++;

            // Separation: Move away from nearby agents
            if (distance < SEPARATION_RADIUS) {
                separation += m_pos - agent->getPosition();
                separationCount++;
            }
        }
    }

    // Apply weights to balance the influence of each behavior
    float cohesionWeight = 0.5f; // Adjust weights as needed
    float alignmentWeight = 0.5f;
    float separationWeight = 1.0f;

    if (cohesionCount > 0) {
        cohesion /= static_cast<float>(cohesionCount);
        cohesion = seek(cohesion) * cohesionWeight;
    }

    if (alignmentCount > 0) {
        alignment /= static_cast<float>(alignmentCount); // Average velocity
        normalize(alignment *= MAX_SPEED); // Normalize and set magnitude to MAX_SPEED
        alignment -= m_velocity; // Remove current velocity (optional)
        alignment *= alignmentWeight; // Apply weight (optional, if needed)
    }

    if (separationCount > 0) {
        separation /= static_cast<float>(separationCount);
        separation = seek(m_pos + separation) * separationWeight;
    }

    // Apply forces
    sf::Vector2f totalForce = cohesion + alignment + separation;

    // Calculate the magnitude of the total force
    float forceMagnitude = std::sqrt(totalForce.x * totalForce.x + totalForce.y * totalForce.y);

    // Check if the force magnitude is greater than zero to avoid division by zero
    if (forceMagnitude > 0) {
        // Normalize the total force and then scale it by MAX_FORCE
        totalForce = totalForce / forceMagnitude * MAX_FORCE;
    }

    // Update the agent's velocity based on the total force
    m_velocity += totalForce;

    // Limit the agent's velocity to MAX_SPEED
    float velocityMagnitude = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
    if (velocityMagnitude > MAX_SPEED) {
        m_velocity = m_velocity / velocityMagnitude * MAX_SPEED;
    }

    m_pos += m_velocity;

    // Wrap agent position within the screen
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
    float distance = std::sqrt(std::pow(desiredVelocity.x, 2) + std::pow(desiredVelocity.y, 2));
    desiredVelocity = desiredVelocity / distance * MAX_SPEED;

    // Calculate steering force
    sf::Vector2f steering = desiredVelocity - m_velocity;
    if (std::sqrt(std::pow(steering.x, 2) + std::pow(steering.y, 2)) > MAX_FORCE) {
        steering = steering / (float)std::sqrt(std::pow(steering.x, 2) + std::pow(steering.y, 2)) * MAX_FORCE;
    }

    return steering;
}

sf::Vector2f Agent::separate(const std::vector<std::unique_ptr<Agent>>& agents)
{
    sf::Vector2f separationForce(0.0f, 0.0f);

    // Iterate through each neighboring agent
    for (const auto& agent : agents) {
        if (agent.get() != this) {
            // Calculate the distance between this agent and the neighboring agent
            sf::Vector2f diff = m_pos - agent->getPosition();
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            // If the neighboring agent is too close, add a repulsive force
            if (distance < SEPARATION_RADIUS) {
                // Scale the separation force based on the distance and direction
                sf::Vector2f scaledDiff = diff / distance;
                float scaledDistance = std::min(distance, SEPARATION_RADIUS);
                separationForce += scaledDiff * (1.0f - scaledDistance / SEPARATION_RADIUS);
            }
        }
    }

    // Normalize the separation force
    float separationForceMagnitude = std::sqrt(separationForce.x * separationForce.x + separationForce.y * separationForce.y);
    if (separationForceMagnitude > 0) {
        separationForce /= separationForceMagnitude;
    }

    return separationForce;
}