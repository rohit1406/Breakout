//
//  PowerUp.cpp
//  Breakout
//
//  Created by Apple on 11/07/21.
//

#include "PowerUp.hpp"

// The size of a PowerUp block
const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
// Velocity a PowerUp block has when spawned
const glm::vec2 POWERUP_VELOCITY(0.0f, 150.0f);

// Constructor/Destructor
PowerUp::PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture)
    : GameObject(position, POWERUP_SIZE, texture, color, POWERUP_VELOCITY), type(type), duration(duration), activated(){
    
}

PowerUp::~PowerUp(){
    
}

