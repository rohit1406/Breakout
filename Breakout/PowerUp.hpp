//
//  PowerUp.hpp
//  Breakout
//
//  Created by Apple on 11/07/21.
//

#ifndef PowerUp_hpp
#define PowerUp_hpp

#include <stdio.h>

#include <glm/glm.hpp>

#include "GameObject.hpp"
#include "Texture.hpp"

class PowerUp : public GameObject{
public:
    // power up states
    std::string type;
    float duration;
    bool activated;
    
    // Constructor/Destructor
    PowerUp(std::string, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture);
    ~PowerUp();
};
#endif /* PowerUp_hpp */
