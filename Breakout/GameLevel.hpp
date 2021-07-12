//
//  GameLevel.hpp
//  Breakout
//
//  Created by Apple on 05/07/21.
//

#ifndef GameLevel_hpp
#define GameLevel_hpp

#include <stdio.h>

#include <glm/glm.hpp>
#include <vector>
#include <fstream>
#include <sstream>

#include "GameObject.hpp"
#include "ResourceManager.hpp"

class GameLevel{
public:
    // Level States
    std::vector<GameObject> bricks;
    
    // FUNCTIONS
    // --Constructor/Destructors
    GameLevel();
    ~GameLevel();
    
    // --Loader Function
    void load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    
    // --Render Function
    void draw(SpriteRenderer &renderer);
    
    // --Checker Function
    // Check if level is complete (all non-solid bricks are destroyed)
    bool isComplete();
    
private:
    // Initialize level from tile data
    void init(std::vector<std::vector<unsigned int>> tileData,
              unsigned int levelWidth, unsigned int levelHeight);
};
#endif /* GameLevel_hpp */
