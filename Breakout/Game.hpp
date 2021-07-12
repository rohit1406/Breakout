//
//  Game.hpp
//  Breakout
//
//  Created by Apple on 28/06/21.
//

#ifndef Game_hpp
#define Game_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <tuple>

#include "Commons.h"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include "GameLevel.hpp"
#include "BallObject.hpp"
#include "ParticleGenerator.hpp"
#include "PostProcessor.hpp"
#include "PowerUp.hpp"
#include "SoundEngine.hpp"
#include "TextRenderer.hpp"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Initial size of the player
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

enum Direction{
  UP, RIGHT, DOWN, LEFT
};

// a collision occurred, what direction it occurred, and the difference vector R
typedef std::tuple<bool, Direction, glm::vec2> Collision;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState state;
    bool keys[1024];
    bool keysProcessed[1024];
    unsigned int width, height;
    
    std::vector<GameLevel> levels;
    unsigned int level;
    
    std::vector<PowerUp> powerUps;
    
    unsigned int lives;
    
    // Functions
    // -- Constructor/Destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    
    // initialize game state (load all shaders/textures/levels)
    void init();
    
    // game loop
    void processInput(float dt);
    void update(float dt);
    void render();
    
    void doCollisions();
    
    // PowerUp Functions
    void spawnPowerUps(GameObject &block);
    void updatePowerUp(float dt);
    
private:
    //glm::mat4 projection;
    GLuint rotate;
    bool checkCollission(GameObject &one, GameObject &two);
    Collision checkCollission(BallObject &one, GameObject &two);
    
    Direction vectorDirection(glm::vec2 target);
    
    void resetLevel();
    void resetPlayer();
    
    // PowerUp Functions
    bool shouldSpawn(unsigned int chance);
    void activatePowerUp(PowerUp &powerUp);
    bool isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);
};

#endif /* Game_hpp */
