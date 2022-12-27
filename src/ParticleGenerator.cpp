#include "../include/header/ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, unsigned int amount)
    : particleShader(shader), particleTexture(texture), amount(amount)
{
     this->init();
}

void ParticleGenerator::Update(float dt, unsigned int newParticles, glm::vec3 offset,  glm::vec3 position)
{
    // Add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], offset, position);
    }
    // Update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle &p = this->particles[i];
        p.Life -= dt;
        // p.Life -= dt*((rand() % 100) / 100.0f); // reduce life randomly
        if (p.Life > 0.0f) // check if particle alive
        {	
            p.Position -= p.Velocity * dt; 
            // p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::Draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    // this->particleShader.Activate();
    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            this->particleShader.setVec3("offset", particle.Position);
            this->particleShader.setVec4("color", particle.Color);
            this->particleTexture.Bind();
            glBindVertexArray(this->particleVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    unsigned int particleVBO;
    float particle_quad[] = {
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f
    }; 
    glGenVertexArrays(1, &this->particleVAO);
    glGenBuffers(1, &particleVBO);
    glBindVertexArray(this->particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 6, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);   
    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i){
        if (this->particles[i].Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, glm::vec3 offset,glm::vec3 position)
{
    glm::vec3 yes = glm::vec3(((rand() % 100) - 50) / 100.0f, ((rand() % 100) - 50) / 100.0f, ((rand() % 100) - 50) / 100.0f);
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = position + yes + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 2.0f + ((rand() % 100) / 100.0f);
    particle.Velocity =  glm::vec3(0.0f,0.1f,0.0f);

}