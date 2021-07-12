//
//  GameObject.hpp
//  Breakout
//
//  Created by Apple on 04/07/21.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>

#include <glm/glm.hpp>

#include "Texture.hpp"
#include "SpriteRenderer.hpp"
/*
 Container object for holding all the state relevant for a single game object entity.
 Each Object in the game likely needs the minimal of state as described with GameObject.
 
 Each object in the game is represented as a GameObject or a derivative of this class. 
 */
class GameObject{
public:
    // Object States
    glm::vec2 position, size, velocity;
    glm::vec3 color;
    float rotation;
    bool isSolid;
    bool destroyed;
    
    // Render States
    Texture2D sprite;
    
    // FUNCTIONS
    // -- CONSTRUCTORS/DESTRUCTORS
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    ~GameObject();
    
    // -- Render Functions
    virtual void draw(SpriteRenderer &renderer);
};

#endif /* GameObject_hpp */
