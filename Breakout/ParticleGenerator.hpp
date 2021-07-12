//
//  ParticleGenerator.hpp
//  Breakout
//
//  Created by Apple on 10/07/21.
//

#ifndef ParticleGenerator_hpp
#define ParticleGenerator_hpp

#include <stdio.h>

#include <glm/glm.hpp>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"
#include "GameObject.hpp"

// Represents Single Particle and it's State
struct Particle{
    glm::vec2 position, velocity;
    glm::vec4 color;
    float life;
    
    Particle()
    : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f){}
};

class ParticleGenerator{
public:
    // FUNCTIONS
    // -- Constructors/Destructors
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    ~ParticleGenerator();
    
    // -- Update Functions
    void update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    
    // -- Render Functions
    void draw();
private:
    // STATES
    std::vector<Particle> particles;
    unsigned int amount;
    
    // -- Render States
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    
    // FUNCTIONS
    // -- Initialize Functions
    void init();        // Initializes buffer and vertex attributes
    
    // -- Utility Functions
    // Returns first unused particle index that is currently unsued e.g. life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    
    // Respawns Particle
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
#endif /* ParticleGenerator_hpp */
