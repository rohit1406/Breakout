//
//  ParticleGenerator.cpp
//  Breakout
//
//  Created by Apple on 10/07/21.
//

#include "ParticleGenerator.hpp"

// FUNCTIONS
// -- Initialize Functions
void ParticleGenerator::init(){        // Initializes buffer and vertex attributes
    // Setup Mesh and Attribute Properties
    float particleQuad[] = {
        // Pos      // TexCoords
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    unsigned int VBO;
    
    // Create VAO and VBO
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Pass Data to Buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);
    
    // Setup mesh attribute
    glVertexAttribPointer(
                          0,                    // Layout Position in Shader
                          4,                    // Number of elements in each line
                          GL_FLOAT,             // Type of data (number)
                          GL_FALSE,             // Normalization not required
                          4 * sizeof(float),    // Offset In Data
                          (void *) 0            // Start Position
                          );
    glEnableVertexAttribArray(0);               // Enable Layout position 0 in Shader
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Create default particle instances
    for(unsigned int i=0; i<this->amount; i++){
        this->particles.push_back(Particle());
    }
}

// -- Utility Functions
unsigned int lastUsedParticle = 0;
// Returns first unused particle index that is currently unsued e.g. life <= 0.0f or 0 if no particle is currently inactive
unsigned int ParticleGenerator::firstUnusedParticle(){
    // First search from last used particle, this will usually return almost instantly
    for(unsigned int i= lastUsedParticle; i<this->amount; i++){
        if(this->particles[i].life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    
    // Otherwise, do a linear search
    for(unsigned int i=0; i<lastUsedParticle; i++){
        if(this->particles[i].life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    
    // all particle are token, override the first one (note that if it repeatedly hits this case, more particles should be reversed)
    lastUsedParticle = 0;
    return 0;
}

// Respawns Particle
void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset){
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.position = object.position + random + offset;
    particle.color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.life = 1.0f;
    particle.velocity = object.velocity * 0.1f;
}

// -- Constructors/Destructors
ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
        : shader(shader), texture(texture), amount(amount){
            this->init();
}

ParticleGenerator::~ParticleGenerator(){
    
}

// -- Update Functions
void ParticleGenerator::update(float dt, GameObject &object, unsigned int newParticles, glm::vec2 offset){
    // add new particle
    for(unsigned int i=0; i<newParticles; i++){
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, offset);
    }
    
    // Update all particles
    for(unsigned int i=0; i<this->amount; i++){
        Particle &p = this->particles[i];
        p.life -= dt;       // Reduce life
        if(p.life > 0.0f){
            // Particle is alive, so update
            p.position -= p.velocity * dt;
            p.color.a -= dt * 2.5f;
        }
    }
}

// -- Render Functions
void ParticleGenerator::draw(){
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    this->shader.use();
    for(Particle particle: this->particles){
        if(particle.life > 0.0f){
            this->shader.setVector2f("uOffset", particle.position);
            this->shader.setVector4f("uColor", particle.color);
            this->texture.bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
