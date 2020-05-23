// Implementation for Player class

#include "entities/player.hpp"
#include "level/level.hpp"

Player::Player(int screenX, int screenY, int gridX, int gridY, 
    SDL_Renderer* renderer) : Entity(screenX, screenY, gridX, gridY),
    startX(screenX), startY(screenY), endX(screenX), endY(screenY) {

    texture.loadFromFile(texturePath, renderer);
}

void Player::handleEvents(const Uint8 * keyStates, Level * level) {
    // Check to start moving or  buffer a move
    if(!moving || (moveProg > 0.85f && !bufferedMove)) {
        checkMovement(keyStates, level);
    }
}

void Player::update(Level * level) {
    if(moving) {
        move();
    }
}

void Player::render(SDL_Renderer* renderer) {
    texture.render(screenX, screenY, renderer);
}

void Player::checkMovement(const Uint8* keyStates, Level * level) {
    // If already moving, buffer a move
    if(moving) {
        bufferedMove = true;
    }

    if(keyStates[SDL_SCANCODE_UP]) {
        if(gridY > 0) {
            if(!moving) {
                initMovement(DIR_UP);
            } else {
                bufferedY = endY - texture.getWidth();
                bufferedDir = DIR_UP;
            }
        }
    } else if (keyStates[SDL_SCANCODE_DOWN]) {
        if(gridY < level->getGridHeight() - 1) {
            if(!moving) {
                endY += texture.getWidth();
                gridY += 1;
                initMovement();
            } else {
                bufferedY = endY + texture.getWidth();
                bufferedDir = DIR_DOWN;
            }
        }
    } else if (keyStates[SDL_SCANCODE_LEFT]) {
        if(gridX > 0) {
            if(!moving) {
                endX -= texture.getWidth();
                gridX -= 1;
                initMovement();
            } else {
                bufferedX = endX - texture.getWidth();
                bufferedDir = DIR_LEFT;
            }
        } 
    } else if (keyStates[SDL_SCANCODE_RIGHT]) {
        if(gridX < level->getGridWidth() - 1) {
            if(!moving) {
                endX += texture.getWidth();
                gridX += 1;
                initMovement();
            } else {
                bufferedX = endX + 48;
                bufferedDir = DIR_RIGHT;
            }
        }       
    }
}

void Player::initMovement(int direction) {
    if(!moving) {
        moving = true;
        moveStartTime = SDL_GetTicks();
    } else {
        switch(direction) {
            case DIR_UP:
                endY -= texture.getHeight();
                gridY -= 1;
                break;
            case DIR_DOWN:
                endY += texture.getHeight();
                gridY += 1;
                break;
            case DIR_LEFT:
                endX -= texture.getWidth();
                gridX -= 1;
                break; 
            case DIR_RIGHT:
                endX += texture.getHeight();
                gridX += 1;
                break;       
        }
    }
}

void Player::move() {
    if(moveProg < 1.0f) {
        // Update moveProg based on time
        moveProg = PLAYER_VELOCITY * ((SDL_GetTicks() - moveStartTime)/1000.0f);

        // Perform linear interpolation if not yet reached
        std::pair<int, int> newPos = lerp(startX, startY, endX, endY, moveProg);
        screenX = newPos.first;
        screenY = newPos.second;
    } else {
        // Check if a move is buffered
        if(bufferedMove) {
            bufferedMove = false;

            // update grid position
            switch(bufferedDir) {
                case UP: 
                    gridY -= 1;
                    break;
                case DOWN:
                    gridY += 1;
                    break;
                case LEFT:
                    gridX -= 1;
                    break;
                case RIGHT:
                    gridX += 1;
                    break;
            }

            bufferedDir = NONE;

            endX = bufferedX;
            endY = bufferedY;
            moveStartTime = SDL_GetTicks();
        } else {
            moving = false;
            moveStartTime = 0;
            
            // Account for float error
            screenX = endX;
            screenY = endY;
        }

        // Reset movement
        moveProg = 0.f;
        startX = screenX;
        startY = screenY;
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

int Player::getScreenX() { return screenX; }
int Player::getScreenY() { return screenY; }
int Player::getGridX() {return gridX; }
int Player::getGridY() {return gridY; }
float Player::getMoveProg() {return moveProg; }