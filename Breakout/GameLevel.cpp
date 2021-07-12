//
//  GameLevel.cpp
//  Breakout
//
//  Created by Apple on 05/07/21.
//

#include "GameLevel.hpp"

// FUNCTIONS
// --Constructor/Destructors
GameLevel::GameLevel(){
    
}
GameLevel::~GameLevel(){
    
}

// --Loader Function
void GameLevel::load(const char* file, unsigned int levelWidth, unsigned int levelHeight){
    this->bricks.clear();
    LOGGER("Loading Level: "+std::string(file));
    // Load bricks data in two dimentional array
    unsigned int tileCode;
    //GameLevel level;
    std::string line;
    std::ifstream fStream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if(fStream){
        // Read each line from a level file
        while(std::getline(fStream, line)){
            std::istringstream sStream(line);
            std::vector<unsigned int> row;
            
            // Read each word separated by spaces
            while(sStream >> tileCode){
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
        
        if(tileData.size() > 0){
            this->init(tileData, levelWidth, levelHeight);
        }
    }
}

// --Render Function
void GameLevel::draw(SpriteRenderer &renderer){
    for(GameObject &tile: this->bricks){
        if(!tile.destroyed){
            tile.draw(renderer);
        }
    }
}

// --Checker Function
// Check if level is complete (all non-solid bricks are destroyed)
bool GameLevel::isComplete(){
    for (GameObject &tile : this->bricks)
            if (!tile.isSolid && !tile.destroyed)
                return false;
        return true;
}

// Initialize level from tile data
void GameLevel::init(std::vector<std::vector<unsigned int>> tileData,
                     unsigned int levelWidth, unsigned int levelHeight){
    // Calculate Dimentions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size();
    float unitWidth = levelWidth / static_cast<float>(width);
    float unitHeight = levelHeight / height;
    
    // Initialize level tiles based on tileData
    for(unsigned int y=0; y<height; y++){
        for(unsigned int x=0; x<width; x++){
            // Check block type from level data (2D level array)
            if(tileData[y][x] == 1){        // Solid Brick
                glm::vec2 pos(unitWidth * x, unitHeight * y);
                glm::vec2 size(unitWidth, unitHeight);
                GameObject obj(pos, size, ResourceManager::getTexture("block_solid"),
                               glm::vec3(0.8f, 0.8f, 0.7f));
                obj.isSolid = true;
                this->bricks.push_back(obj);
            }else if(tileData[y][x] > 1) {  // Colorful Destroyable
                glm::vec3 color = glm::vec3(1.0f);  // Original: white
                if(tileData[y][x] == 2){
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                }else if(tileData[y][x] == 3){
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                }else if(tileData[y][x] == 4){
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                }else if(tileData[y][x] == 5){
                    color = glm::vec3(1.0f, 0.5f, 0.0f);
                }
                
                glm::vec2 pos(unitWidth * x, unitHeight * y);
                glm::vec2 size(unitWidth, unitHeight);
                GameObject obj(pos, size, ResourceManager::getTexture("block"),
                               color);
                this->bricks.push_back(obj);
            }
        }
    }
}
