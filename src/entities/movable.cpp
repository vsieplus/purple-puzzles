#include "entities/movable.hpp"
#include "level/level.hpp"

Movable::Movable(int screenX, int screenY, int gridX, int gridY, int velocity,
    std::shared_ptr<Sprite> entitySprite, int movableParity) : 
    Entity(screenX, screenY, gridX, gridY, entitySprite), startX(screenX),
    startY(screenY), endX(screenX), endY(screenY), velocity(velocity),
    movableParity(movableParity) {}

void Movable::update(Level * level, float delta) {
    // Update entity position for movement if currently moving
    if(moving) {
        move(level, delta);
    } else if(moveDir != DIR_NONE) {
        // try to initialize movement if moveDir is not DIR_NONE
        initMovement(moveDir, level);
        moveDir = DIR_NONE;
    }
}

// Helper function for initMovement
void Movable::initMovement(int direction, Level * level) {
    switch(direction) {
        case DIR_UP:
            initMovement(0, -entitySprite->getHeight(), 0, -1, direction, level);
            break;
        case DIR_DOWN:
            initMovement(0, entitySprite->getHeight(), 0, 1, direction, level);
            break;
        case DIR_LEFT:
            initMovement(-entitySprite->getWidth(), 0, -1, 0, direction, level);
            break;
        case DIR_RIGHT:
            initMovement(entitySprite->getWidth(), 0, 1, 0, direction, level);
            break;                        
    }
}

/**
 * @brief attempt to initialize player movement to the specified new
 *        position. Returns early if unable to
 */
void Movable::initMovement(int xPosChange, int yPosChange, int xGridChange, 
    int yGridChange, int direction, Level * level) {

    int newGridX = gridX + xGridChange;
    int newGridY = gridY + yGridChange;

    // Check for collisions or invalid tile movement (same tile Parity)
    if(checkCollision((level->getMap()), newGridX, newGridY) || 
        movableParity == level->getMap().getTileParity(newGridX, newGridY)) {
        return;
    }

    // Flip map tiles
    if(direction != DIR_NONE) {
        level->flipMapTiles(gridX, gridY, movableParity);
    }

    // Reset movement
    moveProg = 0.f;
    startX = screenX;
    startY = screenY;

    moving = true;
    
    // Update player position
    endX += xPosChange;
    endY += yPosChange;

    int oldGridX = gridX;
    int oldGridY = gridY;
    
    gridX = newGridX;
    gridY = newGridY; 

    // Update pos. of ptr in the level grid
    level->moveGridElement(oldGridX, oldGridY, gridX, gridY);
}

// Move the player
void Movable::move(Level * level, float delta) {
    if(moveProg < 1.0f) {
        // Update moveProg based on time
        moveProg += velocity * (delta/1000.f);

        // Perform linear interpolation if not yet reached
        std::pair<int, int> newPos = lerp(startX, startY, endX, endY, moveProg);

        // For possible overshoot from float error, set to end position
        screenX = abs(newPos.first - startX) < abs(endX - startX) ? newPos.first : endX;
        screenY = abs(newPos.second - startY) < abs(endY - startY) ? newPos.second : endY;

        renderArea.x = screenX;
        renderArea.y = screenY;
    } else {
        // When current move is finished check if a move is buffered
        if(bufferedDir != DIR_NONE) {
            initMovement(bufferedDir, level);
            bufferedDir = DIR_NONE;
        } else {
            moving = false;
        }
    }
}

// Linear interpolation from current position to <endX, endY>
std::pair<int,int> Movable::lerp(int startX, int startY, int endX,
    int endY, float t) {

    float xChange = endX - startX;
    float yChange = endY - startY;

    int newX = startX + t * xChange;
    int newY = startY + t * yChange;

    return std::make_pair(newX, newY);
}

// set an entity's move/buffered direction if not already set
void Movable::setMoveDir(Direction direction) {
    if(moveDir == DIR_NONE) {
        moveDir = direction;
    } else if(bufferedDir == DIR_NONE) {
        bufferedDir = direction;
    }
}

float Movable::getMoveProg() const {
    return moveProg; 
}