//
//  BallObject.cpp
//  Breakout
//
//  Created by Apple on 05/07/21.
//

#include "BallObject.hpp"

BallObject::BallObject()
    : GameObject(), radius(12.5f), stuck(true), sticky(false), passThrough(false){
    
}
BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), radius(radius), stuck(true), sticky(false), passThrough(false){
    
}
BallObject::~BallObject(){
    
}

glm::vec2 BallObject::move(float dt, unsigned int windowWidth){
    if(!this->stuck){
        // Move the ball
        this->position += this->velocity * dt;
        // check if outside window bound; if so reverse the velocity and restore at correct position
        if(this->position.x <= 0.0f){
            this->velocity.x = -this->velocity.x;
            this->position.x = 0.0f;
        }else if(this->position.x + this->size.x >= windowWidth){
            this->velocity.x = -this->velocity.x;
            this->position.x = windowWidth - this->size.x;
        }
        
        if(this->position.y <= 0.0f){
            this->velocity.y = -this->velocity.y;
            this->position.y = 0.0f;
        }
    }
    return this->position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity){
    this->position = position;
    this->velocity = velocity;
    this->stuck = true;
    this->sticky = false;
    this->passThrough = false;
}
