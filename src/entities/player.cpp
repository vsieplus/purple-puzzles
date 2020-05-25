// Implementation for Player class

#include "entities/player.hpp"
#include "level/level.hpp"

Player::Player(int screenX, int screenY, int gridX, int gridY, 
    SDL_Renderer* renderer) : Entity(screenX, screenY, gridX, gridY, "res/spritesheets/sprites/portal_lv0.png", renderer),
    startX(screenX), startY(screenY), endX(screenX), endY(screenY) {
}

void Player::handleEvents(const Uint8 * keyStates, Level * level) {
    // Check if player wants to start moving or  buffer a move
    if(!moving || (moveProg > MOVEMENT_BUFFER && !bufferedMove)) {
        checkMovement(keyStates, level);
    }
}

void Player::update(Level * level) {
    // Actually update player position for movement if currently moving
    if(moving) {
        move(level);
    } else if(moveDir != DIR_NONE) {
        // initialize movement if moveDir is not DIR_NONE
        initMovement(moveDir, level);
        moveDir = DIR_NONE;
    }
}

void Player::render(SDL_Renderer* renderer) const {
    texture.render(screenX, screenY, renderer);
}

void Player::checkMovement(const Uint8* keyStates, Level * level) {
    Direction newDir = DIR_NONE;

    // Check for key inputs
    if(keyStates[SDL_SCANCODE_UP]) {
        newDir = DIR_UP;
    } else if (keyStates[SDL_SCANCODE_DOWN]) {
        newDir = DIR_DOWN;
    } else if (keyStates[SDL_SCANCODE_LEFT]) {
        newDir = DIR_LEFT;
    } else if (keyStates[SDL_SCANCODE_RIGHT]) {
        newDir = DIR_RIGHT;       
    }
    
    // Update the buffered Direction if already moving, or set new moveDir
    if(moving) {
        bufferedDir = newDir;
    } else {
        moveDir = newDir;
    }
}

// Helper function for initMovement
void Player::initMovement(int direction, Level * level) {
    switch(direction) {
        case DIR_UP:
            initMovement(0, -texture.getHeight(), 0, -1, level);
            break;
        case DIR_DOWN:
            initMovement(0, texture.getHeight(), 0, 1, level);
            break;
        case DIR_LEFT:
            initMovement(-texture.getWidth(), 0, -1, 0, level);
            break;
        case DIR_RIGHT:
            initMovement(texture.getWidth(), 0, 1, 0, level);
            break;                        
    }
}

/**
 * @brief attempt to initialize player movement to the specified new
 *        position
 */
void Player::initMovement(int xPosChange, int yPosChange, int xGridChange, 
    int yGridChange, Level * level) {

    // Check for collisions or invalid tile movement
    if(checkCollision(level, gridX + xGridChange, gridY + yGridChange)) {
        return;
    } else {    
        // Reset movement
        moveProg = 0.f;
        startX = screenX;
        startY = screenY;

        moving = true;
        moveStartTime = SDL_GetTicks();
        
        // Update player position
        endX += xPosChange;
        endY += yPosChange;

        int oldGridX = gridX;
        int oldGridY = gridY;
        
        gridX += xGridChange;
        gridY += yGridChange; 

        // Update pos. of ptr in the level grid
        level->setGridElement(oldGridX, oldGridY, gridX, gridY);
    }
}

// Move the player
void Player::move(Level * level) {
    if(moveProg < 1.0f) {
        // Update moveProg based on time
        moveProg = PLAYER_VELOCITY * ((SDL_GetTicks() - moveStartTime)/1000.0f);

        // Perform linear interpolation if not yet reached
        std::pair<int, int> newPos = lerp(startX, startY, endX, endY, moveProg);
        screenX = newPos.first;
        screenY = newPos.second;
    } else {
        // Check if a move is buffered
        if(bufferedDir != DIR_NONE) {
            initMovement(bufferedDir, level);
            bufferedDir = DIR_NONE;
        } else {
            moving = false;
            moveStartTime = 0;
            
            // Account for float error
            screenX = endX;
            screenY = endY;
        }
    }
        
}

// Linear interpolation from current position to <endX, endY>
std::pair<int,int> Player::lerp(int startX, int startY, int endX,
    int endY, float t) 
{
    float xChange = endX - startX;
    float yChange = endY - startY;

    int newX = startX + t * xChange;
    int newY = startY + t * yChange;

    return std::make_pair(newX, newY);
}

float Player::getMoveProg() const {
    return moveProg; 
}