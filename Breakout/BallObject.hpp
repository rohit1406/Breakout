//
//  BallObject.hpp
//  Breakout
//
//  Created by Apple on 05/07/21.
//

#ifndef BallObject_hpp
#define BallObject_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.hpp"
#include "Texture.hpp"

class BallObject: public GameObject{
public:
    // Ball States
    float radius;
    bool stuck;
    
    // -- PowerUp States
    // If the sticky effect is activated, the ball will end up stuck to the player paddle whenever it collides; the user then has to press the spacebar again to release the ball.
    bool sticky;
    
    // When the ball's PassThrough property is set to true we do not perform any collision resolution on the non-solid bricks.
    bool passThrough;
    
    BallObject();
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
    ~BallObject();
    
    glm::vec2 move(float dt, unsigned int windowWidth);
    void reset(glm::vec2 position, glm::vec2 velocity);
};
#endif /* BallObject_hpp */
