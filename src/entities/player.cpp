// Implementation for Player class

#include <stdlib.h>

#include "entities/player.hpp"
#include "level/level.hpp"

Player::Player(int screenX, int screenY, int gridX, int gridY, 
    std::shared_ptr<Sprite> entitySprite) : 
    Movable(screenX, screenY, gridX, gridY, PLAYER_VELOCITY, entitySprite,
        PARITY_PURPLE) {}

void Player::handleEvents(const Uint8 * keyStates, Level * level) {
    // Check if player wants to start moving or  buffer a move
    if(!moving || (moveProg > MOVEMENT_BUFFER && bufferedDir == DIR_NONE)) {
        checkMovement(keyStates, level);
    }
}

void Player::update(Level * level, float delta) {
    // Actually update player position for movement if currently moving
    if(moving) {
        move(level, delta);
    } else if(moveDir != DIR_NONE) {
        // initialize movement if moveDir is not DIR_NONE
        initMovement(moveDir, level);
        moveDir = DIR_NONE;
    }
    
    // Update player parity
    //tileParity = level->getMap().getTileParity(gridX, gridY);
}

void Player::render(SDL_Renderer* renderer) const {
    Entity::render(renderer);
    
    // render other player effects
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