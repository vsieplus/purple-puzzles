// Implementation for Player class

#include <stdlib.h>

#include "entities/player.hpp"
#include "entities/receptor.hpp"
#include "entities/diamond.hpp"
#include "level/level.hpp"

const std::string Player::PLAYER_SHAPE = "player";

Player::Player(int screenX, int screenY, int gridX, int gridY, int parity,
    std::shared_ptr<Sprite> entitySprite) :
    Movable(screenX, screenY, gridX, gridY, PLAYER_VELOCITY, parity, entitySprite) {}

void Player::handleEvents(const Uint8 * keyStates, Level * level) {
    // Check if player wants to start moving or buffer a move, when not boosted
    if((!moving || (moveProg > MOVEMENT_BUFFER && bufferedDir == DIR_NONE)) &&
        boostPower == 0) {
        checkMovement(keyStates, level);
    }
}

void Player::update(Level * level, float delta) {
    // try pushing diamond
    if(pushDir != DIR_NONE) {
        pushDiamond(level);
    }

    // update player movement
    Movable::update(level, delta);

    // Update player parity
    //tileParity = level->getTileParity(gridX, gridY);
}

void Player::render(SDL_Renderer* renderer) const {
    Movable::render(renderer);
    
    // render other player effects
}

void Player::checkMovement(const Uint8 * keyStates, Level * level) {
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

    // update push-dir
    pushDir = newDir;
}

void Player::pushDiamond(Level * level) {
    std::pair<int, int> pushCoords = getCoords(pushDir);

    // check if entity at the coordinate is a diamond
    auto diamond = level->getGridElement<Diamond>(pushCoords.first,
        pushCoords.second);

    // set move direction of diamond if not already merging w/receptor
    if(diamond.get() && !diamond->isMerging()) {
        // set the move direction of the diamond
        diamond->setMoveDir(pushDir);
    }

    // reset pushDir
    pushDir = DIR_NONE;
}

// check for exit/determine if level is complete
void Player::checkExit(Level * level, Direction direction) {
    auto coords = getCoords(direction);
    auto exit = level->getGridElement<Receptor>(coords.first, coords.second);

    // if receptor is of player shape, proceed to check if game over
    if(exit.get() && exit->getShape() == PLAYER_SHAPE) {

    }
}