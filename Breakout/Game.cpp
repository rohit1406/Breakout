//
//  Game.cpp
//  Breakout
//
//  Created by Apple on 28/06/21.
//

#include "Game.hpp"

SpriteRenderer *renderer;
GameObject *player;
ParticleGenerator *particles;
PostProcessor *effect;

// Ball related
// Initial velocity of ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of ball object
const float BALL_RADIUS = 12.5f;
BallObject *ball;

SoundEngine *soundEngine;
TextRenderer *text;

float shakeTime = 0.0f;

// -- Constructor/Destructor
Game::Game(unsigned int Width, unsigned int Height)
            : state(GAME_MENU), keys(), keysProcessed(),width(Width), height(Height),level(0), lives(3){
    rotate = 0;
}

Game::~Game(){
    if(text){
        delete text;
    }
    
    soundEngine->drop();
    
    if(soundEngine){
        delete soundEngine;
    }
    
    if(particles){
        delete particles;
    }
    
    if(effect){
        delete effect;
    }
    
    if(renderer){
        delete renderer;
    }
    
    if(player){
        delete player;
    }
    
    if(ball){
        delete ball;
    }
}

// initialize game state (load all shaders/textures/levels)
void Game::init(){
    // Load Shaders
    ResourceManager::loadShader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::loadShader("resources/shaders/particle.vs", "resources/shaders/particle.fs", nullptr, "particle");
    ResourceManager::loadShader("resources/shaders/postprocessing.vs", "resources/shaders/postprocessing.fs", nullptr, "postprocesing");
    
    // Configure Shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);
    
    ResourceManager::getShader("sprite").use().setInteger("usSpriteImage", 0);
    ResourceManager::getShader("sprite").setMatrix4("uProjection", projection);
    
    ResourceManager::getShader("particle").use().setInteger("usSpriteImage", 0);
    ResourceManager::getShader("particle").setMatrix4("uProjection", projection);
    
    // Load Textures
    ResourceManager::loadTexture("resources/textures/background.jpeg", false, "background");
    ResourceManager::loadTexture("resources/textures/awesomeface.png", true, "face");
    ResourceManager::loadTexture("resources/textures/block.png", false, "block");
    ResourceManager::loadTexture("resources/textures/block_solid.png", false, "block_solid");
    ResourceManager::loadTexture("resources/textures/paddle.png", true, "paddle");
    ResourceManager::loadTexture("resources/textures/particle.png", true, "particle");
    // PowerUp Textures
    ResourceManager::loadTexture("resources/textures/powerup_speed.png", true, "powerup_speed");
    ResourceManager::loadTexture("resources/textures/powerup_sticky.png", true, "powerup_sticky");
    ResourceManager::loadTexture("resources/textures/powerup_increase.png", true, "powerup_increase");
    ResourceManager::loadTexture("resources/textures/powerup_confuse.png", true, "powerup_confuse");
    ResourceManager::loadTexture("resources/textures/powerup_chaos.png", true, "powerup_chaos");
    ResourceManager::loadTexture("resources/textures/powerup_passthrough.png", true, "powerup_passthrough");
    
    
    // Set Render Specific Controls
    Shader shader = ResourceManager::getShader("sprite");
    renderer =new SpriteRenderer(shader);
    
    Shader particleShader = ResourceManager::getShader("particle");
    Texture2D particleTexture = ResourceManager::getTexture("particle");
    particles = new ParticleGenerator(particleShader, particleTexture, 500);
    
    Shader postprocesingShader = ResourceManager::getShader("postprocesing");
    effect = new PostProcessor(postprocesingShader, this->width, this->height);
    
    
    // Load Levels
    GameLevel one;
    one.load("resources/levels/one.lvl", this->width, this->height / 2);
    GameLevel two;
    two.load("resources/levels/two.lvl", this->width, this->height / 2);
    GameLevel three;
    three.load("resources/levels/three.lvl", this->width, this->height / 2);
    GameLevel four;
    four.load("resources/levels/four.lvl", this->width, this->height / 2);
    this->levels.push_back(one);
    this->levels.push_back(two);
    this->levels.push_back(three);
    this->levels.push_back(four);
    this->level = 0;
    
    // Configure Gameobject
    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                    this->height - PLAYER_SIZE.y);
    player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));
    
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));
    
    // Init Sound Engine
    soundEngine = new SoundEngine();
    soundEngine->play2D("resources/audio/breakout.mp3", true);
    
    // Init Text Renderer
    text = new TextRenderer(this->width, this->height);
    
    // resources/fonts/OCRAEXT.TTF
    text->load("/System/Library/Fonts/SFCompactText.ttf", 24);
    LOGGER("Game Initialised");
}

// game loop
void Game::processInput(float dt){
    if(this->state == GAME_MENU){
        if(this->keys[GLFW_KEY_ENTER] && !this->keysProcessed[GLFW_KEY_ENTER]){
            this->state = GAME_ACTIVE;
            this->keysProcessed[GLFW_KEY_ENTER] = true;
        }
        
        if(this->keys[GLFW_KEY_W] && !this->keysProcessed[GLFW_KEY_W]){
            this->level  = (this->level + 1) % 4;
            this->keysProcessed[GLFW_KEY_W] = true;
        }
        
        if(this->keys[GLFW_KEY_S] && !this->keysProcessed[GLFW_KEY_S]){
            if(this->level > 0){
                --this->level;
            }else{
                this->level = 3;
            }
            this->keysProcessed[GLFW_KEY_S] = true;
        }
    }
    
    if(this->state == GAME_WIN){
        if(this->keys[GLFW_KEY_ENTER]){
            this->keysProcessed[GLFW_KEY_ENTER] = true;
            effect->chaos = false;
            this->state = GAME_MENU;
        }
    }
    
    if(this->state == GAME_ACTIVE){
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if(this->keys[GLFW_KEY_A]){
            if(player->position.x >= 0.0f){
                player->position.x -= velocity;
                if(ball->stuck){
                    ball->position.x -= velocity;
                }
            }
        }
        
        if(this->keys[GLFW_KEY_D]){
            if(player->position.x <= this->width - player->size.x){
                player->position.x += velocity;
                if(ball->stuck){
                    ball->position.x += velocity;
                }
            }
        }
        
        if(this->keys[GLFW_KEY_SPACE]){
            ball->stuck = false;
        }
    }
    
    
}

void Game::update(float dt){
    rotate++;
    if(rotate >= 360){
        rotate = rotate - 360;
    }
    
    ball->move(dt, this->width);
    // Check for collissions
    this->doCollisions();
    
    // Update Particles
    particles->update(dt, *ball, 2, glm::vec2(ball->radius / 2.0f));
    
    // Update PowerUps
    this->updatePowerUp(dt);
    
    // reduce shake time
    if(shakeTime > 0.0f){
        shakeTime -= dt;
        if(shakeTime <= 0.0f){
            effect->shake = false;
        }
    }
    
    // Reset the game if the ball reaches the bottom of the edge
    if(ball->position.y >= this->height){
        --this->lives;
        
        // did the player lose all his lives? : Game over
        if(this->lives == 0){
            this->resetLevel();
            this->state = GAME_MENU;
        }
        this->resetPlayer();
    }
    
    // Winning the Game
    if(this->state == GAME_ACTIVE  && this->levels[this->level].isComplete()){
        this->resetLevel();
        this->resetPlayer();
        effect->chaos = true;
        this->state = GAME_WIN;
    }
}

void Game::render(){
    if(this->state == GAME_ACTIVE || this->state == GAME_MENU || this->state == GAME_WIN){
            // begin rendering to postprocessing framebuffer
            effect->beginRender();
            // Draw Background
            Texture2D texture = ResourceManager::getTexture("background");
            renderer->drawSprite(texture,
                                glm::vec2(0, 0),  // Position
                                glm::vec2(this->width, this->height)  // Size
                                 );
            // Draw Level
            this->levels[this->level].draw(*renderer);
            
            // Draw Player
            player->draw(*renderer);
            
            
            // Render PowerUps
            for(PowerUp &powerUp: this->powerUps){
                if(!powerUp.destroyed){
                    powerUp.draw(*renderer);
                }
            }
            
            // draw particles
            particles->draw();
            
            // Draw Ball
            ball->draw(*renderer);
            
            
            // end rendering to postprocessing framebuffer
            effect->endRender();
            // render postprocessing quad
            effect->render(glfwGetTime());
            
            std::stringstream ss;
            ss << this->lives;
            text->renderText("Lives: "+ss.str()+" & Current Level: "+std::to_string(this->level), 5.0f, 5.0f, 1.0f);
    }
    
    if(this->state == GAME_MENU){
        text->renderText("Press ENTER to start", 250.0f, height / 2, 1.0f);
        text->renderText("Press W or S to select level", 245.0f, height / 2 + 20.0f, 0.75f);
    }
    
    if (this->state == GAME_WIN)
    {
        text->renderText(
            "You WON!!!", 320.0, height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
        );
        text->renderText(
            "Press ENTER to retry or ESC to quit", 130.0, height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
    }
}

void Game::doCollisions(){
    for(GameObject &box: this->levels[this->level].bricks){
        if(!box.destroyed){
            Collision collision = checkCollission(*ball, box);
            if(std::get<0>(collision)){ // if collision is true
                // Destroy the block if not solid
                if(!box.isSolid){
                    box.destroyed = true;
                    this->spawnPowerUps(box);
                    soundEngine->play2D("resources/audio/bleep.mp3", false);
                }else{
                    // If block is solid, enable shake effect
                    shakeTime = 0.05f;
                    effect->shake = true;
                    soundEngine->play2D("resources/audio/solid.wav", false);
                }
                
                // Collision Resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diffVector = std::get<2>(collision);
                
                if(!(ball->passThrough && !box.isSolid)){
                    if(dir == LEFT || dir == RIGHT){            // Horizontal Collision
                        ball->velocity.x = -ball->velocity.x;   // Reverse horizontal velocity
                        // Relocate
                        float penetration = ball->radius - std::abs(diffVector.x);
                        if(dir == LEFT){
                            ball->position.x += penetration;    // Move ball to right
                        }else{
                            ball->position.x -= penetration;    // Move ball to left
                        }
                    }else{                                      // Vertical Collision
                        ball->velocity.y = -ball->velocity.y;   // Reverse vertical velocity
                        // Relocate
                        float penetration = ball->radius - std::abs(diffVector.y);
                        if(dir == UP){
                            ball->position.y -= penetration;    // Move ball back up
                        }else{
                            ball->position.y += penetration;    // Move ball back down
                        }
                    }
                }
            }
        }
    }
    
    
    // Power Ups
    for(PowerUp &powerUp: this->powerUps){
        if(!powerUp.destroyed){
            if(powerUp.position.y >= this->height){
                powerUp.destroyed = true;
            }
            
            if(checkCollission(*player, powerUp)){
                // Collided with player, now activate powerup
                activatePowerUp(powerUp);
                powerUp.destroyed = true;
                powerUp.activated = true;
                soundEngine->play2D("resources/audio/powerup.wav", false);
            }
        }
    }
    
    // and finally check collisions for player pad (unless stuck)
    // Player Ball Collision
    Collision result = checkCollission(*ball, *player);
    if(!ball->stuck && std::get<0>(result)){
        // Check where it hit the board, and change velocity based on where it hit the board
        float centerBoard = player->position.x + player->size.x / 2.0f;
        float distance = (ball->position.x + ball->radius) - centerBoard;
        float percentage = distance / (player->size.x / 2.0f);
        // then move accordingly
        float strength = 2.0f;
        glm::vec2 oldVelocity = ball->velocity;
        ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        // Fix for sitcky paddle
        //ball->velocity.y = -ball->velocity.y;
        ball->velocity.y = -1.0f * abs(ball->velocity.y);
        ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
        
        ball->stuck = ball->sticky; // add sticky effect
        
        soundEngine->play2D("resources/audio/bleep.wav", false);
    }
}

bool Game::checkCollission(GameObject &one, GameObject &two){
    // Collission x-axis?
    bool collissionX = one.position.x + one.size.x >= two.position.x
    && two.position.x + two.size.x >= one.position.x;
    
    // Collission y-axis?
    bool collissionY = one.position.y + one.size.y >= two.position.y
    && two.position.y + two.size.y >= one.position.y;
    
    // collission only if on both axes
    return collissionX && collissionY;
}

Collision Game::checkCollission(BallObject &one, GameObject &two){
    // Get center point circle first
    glm::vec2 center(one.position + one.radius);
    
    // Calculate AABB info (center, half-extents)
    glm::vec2 aabbHalfExtents(two.size.x/2.0f, two.size.y/2.0f);
    glm::vec2 aabbCenter(two.position.x + aabbHalfExtents.x,
                         two.position.y + aabbHalfExtents.y);
    
    // Get difference vector between both centers
    glm::vec2 difference = center - aabbCenter;
    glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
    
    // Add Clamped value to AABB center and we get the value of box closest to circle
    glm::vec2 closest = aabbCenter + clamped;
    
    // Retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - center;
    if( glm::length(difference) <= one.radius ){
        return std::make_tuple(true, vectorDirection(difference), difference);
    }else{
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
    }
}

Direction Game::vectorDirection(glm::vec2 target){
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f), // UP
        glm::vec2(1.0f, 0.0f), // RIGHT
        glm::vec2(0.0f, -1.0f), // DOWN
        glm::vec2(-1.0f, 0.0f)  // LEFT
    };
    
    float max = 0.0f;
    unsigned int bestMatch = -1;
    for(unsigned int i=0; i<4; i++){
        float dotProduct = glm::dot(glm::normalize(target), compass[i]);
        if(dotProduct > max){
            max = dotProduct;
            bestMatch = i;
        }
    }
    return (Direction) bestMatch;
}

void Game::resetLevel(){
    if(this->level == 0){
        this->levels[0].load("resources/levels/one.lvl", this->width, this->height / 2);
    }else if(this->level == 1){
        this->levels[1].load("resources/levels/two.lvl", this->width, this->height / 2);
    }else if(this->level == 2){
        this->levels[2].load("resources/levels/three.lvl", this->width, this->height / 2);
    }else if(this->level == 3){
        this->levels[3].load("resources/levels/four.lvl", this->width, this->height / 2);
    }
    
    this->lives = 3;
}

void Game::resetPlayer(){
    // Reset Player / ball states
    player->size = PLAYER_SIZE;
    player->position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
    
    // also disable all active powerups
    effect->chaos = effect->confuse = false;
    ball->passThrough = ball->sticky = false;
    player->color = glm::vec3(1.0f);
    ball->color = glm::vec3(1.0f);
}

// PowerUp Functions
void Game::spawnPowerUps(GameObject &block){
    // Positive PowerUps
    // Duration 0.0f means infinite
    if(shouldSpawn(75)){        // 1 in 75 chance
        this->powerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position, ResourceManager::getTexture("powerup_speed")));
    }
    
    if(shouldSpawn(75)){        // 1 in 75 chance
        this->powerUps.push_back(PowerUp("sticky", glm::vec3(0.5f, 0.5f, 1.0f), 20.0f, block.position, ResourceManager::getTexture("powerup_sticky")));
    }
    
    if(shouldSpawn(75)){        // 1 in 75 chance
        this->powerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.position, ResourceManager::getTexture("powerup_passthrough")));
    }
    
    if(shouldSpawn(75)){        // 1 in 75 chance
        this->powerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.position, ResourceManager::getTexture("powerup_increase")));
    }
    
    // Negative PowerUps
    if(shouldSpawn(15)){        // 1 in 15 chance
        this->powerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.position, ResourceManager::getTexture("powerup_confuse")));
    }
    
    if(shouldSpawn(15)){        // 1 in 15 chance
        this->powerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.position, ResourceManager::getTexture("powerup_chaos")));
    }
}

void Game::updatePowerUp(float dt){
    for(PowerUp &powerUp: this->powerUps){
        powerUp.position += powerUp.velocity * dt;
        if(powerUp.activated){
            powerUp.duration -= dt;
            
            if(powerUp.duration <= 0.0f){
                // Remove PowerUp from list ( will later be removed)
                powerUp.activated = false;
                
                // Deactivate Effects
                if(powerUp.type == "sticky"){
                    if(!this->isOtherPowerUpActive(this->powerUps, "sticky")){
                        ball->sticky = false;
                        player->color = glm::vec3(1.0f);
                    }
                }else if(powerUp.type == "pass-through"){
                    if(!this->isOtherPowerUpActive(this->powerUps, "pass-through")){
                        ball->passThrough = false;
                        player->color = glm::vec3(1.0f);
                    }
                }else if(powerUp.type == "confuse"){
                    if(!this->isOtherPowerUpActive(this->powerUps, "confuse")){
                        effect->confuse = false;
                    }
                }else if(powerUp.type == "chaos"){
                    if(!this->isOtherPowerUpActive(this->powerUps, "chaos")){
                        effect->chaos = false;
                    }
                }
            }
        }
    }
    
    // Remove all PowerUps from vector that are destroyed AND !activated (thus either off the map or finished)
    // Note we use a lambda expression to remove each PowerUp which is destroyed and not activated
    this->powerUps.erase(std::remove_if(this->powerUps.begin(), this->powerUps.end(),
            [](const PowerUp &powerUp) { return powerUp.destroyed && !powerUp.activated; }
        ), this->powerUps.end());
}

void Game::activatePowerUp(PowerUp &powerUp){
    if(powerUp.type == "speed"){
        ball->velocity *= 1.2;
    }else if(powerUp.type == "sticky"){
        ball->sticky = true;
        player->color = glm::vec3(1.0f, 0.5f, 1.0f);
    }else if(powerUp.type == "pass-through"){
        ball->passThrough = true;
        player->color = glm::vec3(1.0f, 0.5f, 0.5f);
    }else if(powerUp.type == "pad-size-increase"){
        player->size += 50;
    }else if(powerUp.type == "confuse"){
        if(!effect->chaos){
            // Only active if chaos was not already active
            effect->confuse = true;
        }
    }else if(powerUp.type == "chaos"){
        if(!effect->confuse){
            // Only active if confuse was not already active
            effect->chaos = true;
        }
    }
}

bool Game::shouldSpawn(unsigned int chance){
    unsigned int random = rand() % chance;
    return random == 0;
}

bool Game::isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type){
    for (const PowerUp &powerUp : powerUps)
        {
            if (powerUp.activated)
                if (powerUp.type == type)
                    return true;
        }
    return false;
}
