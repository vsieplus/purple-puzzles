#include "entities/movable.hpp"
#include "level/level.hpp"

Movable::Movable(int screenX, int screenY, int gridX, int gridY, int velocity,
    int parity, std::shared_ptr<Sprite> entitySprite) : 
    Entity(screenX, screenY, gridX, gridY, parity, entitySprite), startX(screenX),
    startY(screenY), endX(screenX), endY(screenY), velocity(velocity) {}

void Movable::update(Level * level, float delta) {
    // update booster if being booster
    if(booster.get()) {
        if(boostPower > 0) {
            booster->update(level, delta);
        } else {
            booster.reset();
        }
    }

    // Update entity position for movement if currently moving
    if(moving) {
        move(level, delta);
    } else if(moveDir != DIR_NONE) {
        // check for boost
        checkBoost(level, moveDir);
        
        // try to initialize movement if moveDir is not DIR_NONE
        initMovement(moveDir, level);
        moveDir = DIR_NONE;
    }
}

void Movable::render(SDL_Renderer* renderer) const {
    if(boostPower > 0 && booster.get()) {
        booster->render(renderer);
    }

    Entity::render(renderer);
}

// Helper function for initMovement
void Movable::initMovement(Direction direction, Level * level) {
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
        case DIR_NONE:
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
    if(checkCollision(level, newGridX, newGridY) || parity == 
       level->getTileParity(newGridX, newGridY)) {
        return;
    }

    // Flip map tiles
    if(direction != DIR_NONE) {
        level->flipMapTiles(gridX, gridY, parity);
    }

    // Reset movement
    moveProg = 0.f;
    startX = renderArea.x;
    startY = renderArea.y;

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
        renderArea.x = abs(newPos.first - startX) < abs(endX - startX) ? newPos.first : endX;
        renderArea.y = abs(newPos.second - startY) < abs(endY - startY) ? newPos.second : endY;
    } else {
        // When current move is finished check if a move is buffered/boosting
        if(bufferedDir != DIR_NONE) {
            checkBoost(level, bufferedDir);
            initMovement(bufferedDir, level);

            bufferedDir = DIR_NONE;
        } else if(boostDir != DIR_NONE) {
            // if currently using boost status, decrease by 1 each time
            if(boostPower > 0) {
                // Check for another boost at next tile when already boosted
                // if there is, avoid boosting 2x in that direction
                if(!checkBoost(level, boostDir)) {
                    initMovement(boostDir, level);
                    boostPower--;            
                }
            } else {
                // otherwise reset boost dir./booster
                booster.reset();
                boostDir = DIR_NONE;
            }
        } else {
            moving = false;
        }
    }
}

// check for a boost in the specified direction, and interact accordingly
bool Movable::checkBoost(Level * level, Direction direction) {
    // check coordinate in direction of move
    auto coords = getCoords(direction);

    auto boost = level->getGridElement<Boost>(coords.first, coords.second);
    
    // if there is a boost in the tile we want to move to, activate it
    if(boost.get()) {
        // remove booster from map/store
        booster = boost;
        level->removeGridElement(boost->getGridX(), boost->getGridY());

        boost->setActivated(true);

        // if currently boosted, finish last boost
        if(boostPower > 0) { 
            initMovement(boostDir, level);    
        }

        // store boost power and direction, overwriting any existing boosts
        boostPower = boost->getPower();
        boostDir = boost->getDirection();
        
        return true;
    }

    return false;
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