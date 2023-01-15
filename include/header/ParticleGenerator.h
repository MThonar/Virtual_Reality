#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include "../include/glad/glad.h"
#include <glm/glm.hpp>

#include "../include/header/Shader.h"
#include "../include/header/texture.h"



// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


// ParticleGenerator acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class ParticleGenerator
{
public:
    // constructor
    ParticleGenerator(Shader shader, unsigned int amount, glm::vec3 velocity);
    // update all particles
    void Update(float dt,  unsigned int newParticles, glm::vec3 offset,  glm::vec3 position, float angle);
    // render all particles
    void Draw();
private:
    // state
    std::vector<Particle> particles;
    unsigned int amount;
    glm::vec3 velocity;
    // render state
    Shader particleShader;
    unsigned int particleVAO;
    // initializes buffer and vertex attributes
    void init();
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle &particle, glm::vec3 offset, glm::vec3 position, float angle);
};

#endif