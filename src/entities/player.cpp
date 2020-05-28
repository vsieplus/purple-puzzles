// Implementation for Player class

#include <stdlib.h>

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
    if(keyStates[SDL_SCANCODE_W]) {
        newDir = DIR_UP;
    } else if (keyStates[SDL_SCANCODE_S]) {
        newDir = DIR_DOWN;
    } else if (keyStates[SDL_SCANCODE_A]) {
        newDir = DIR_LEFT;
    } else if (keyStates[SDL_SCANCODE_D]) {
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
            initMovement(0, -texture.getHeight(), 0, -1, direction, level);
            break;
        case DIR_DOWN:
            initMovement(0, texture.getHeight(), 0, 1, direction, level);
            break;
        case DIR_LEFT:
            initMovement(-texture.getWidth(), 0, -1, 0, direction, level);
            break;
        case DIR_RIGHT:
            initMovement(texture.getWidth(), 0, 1, 0, direction, level);
            break;                        
    }
}

/**
 * @brief attempt to initialize player movement to the specified new
 *        position. Returns early if unable to
 */
void Player::initMovement(int xPosChange, int yPosChange, int xGridChange, 
    int yGridChange, int direction, Level * level) {

    int newGridX = gridX + xGridChange;
    int newGridY = gridY + yGridChange;

    // Check for collisions or invalid tile movement (differing tileParity)
    if(checkCollision(level, newGridX, newGridY) || 
       tileParity != level->getMap().getTileParity(newGridX, newGridY)) {
        return;
    }

    // Flip map tiles
    if(direction != DIR_NONE) {
        level->flipMapTiles(gridX, gridY, direction);
    }

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
    
    gridX = newGridX;
    gridY = newGridY; 

    // Update pos. of ptr in the level grid
    level->setGridElement(oldGridX, oldGridY, gridX, gridY);

    // Update player parity
    tileParity = level->getMap().getTileParity(gridX, gridY);
}

// Move the player
void Player::move(Level * level) {
    if(moveProg < 1.0f) {
        // Update moveProg based on time
        moveProg = PLAYER_VELOCITY * ((SDL_GetTicks() - moveStartTime)/1000.0f);

        // Perform linear interpolation if not yet reached
        std::pair<int, int> newPos = lerp(startX, startY, endX, endY, moveProg);

        // For possible overshoot from float error, set to end position
        screenX = abs(newPos.first - startX) < abs(endX - startX) ? newPos.first : endX;
        screenY = abs(newPos.second - startY) < abs(endY - startY) ? newPos.second : endY;
    } else {
        // Check if a move is buffered
        if(bufferedDir != DIR_NONE) {
            initMovement(bufferedDir, level);
            bufferedDir = DIR_NONE;
        } else {
            moving = false;
            moveStartTime = 0;
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