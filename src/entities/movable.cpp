#include "entities/movable.hpp"
#include "level/level.hpp"

Movable::Movable(int screenX, int screenY, int gridX, int gridY, int velocity,
    int parity, std::shared_ptr<Sprite> entitySprite, std::string movableShape) : 
    Entity(screenX, screenY, gridX, gridY, parity, entitySprite), startX(screenX),
    startY(screenY), endX(screenX), endY(screenY), velocity(velocity), 
    movableShape(movableShape) {}

void Movable::update(Level * level, float delta) {    
    if(moving) {
        // if boosted, update booster
        if(booster.get()) {
            if(boostPower > 0) {
                booster->update(level, delta);
            } else {
                booster.reset();
            }
        }

        // Update entity position for movement if currently moving
        move(level, delta);
    } else if(moveDir != DIR_NONE) {
        // check for boost
        if(!checkBoost(level, moveDir)) {
            // try to initialize movement if moveDir is not DIR_NONE
            initMovement(moveDir, level);
            moveDir = DIR_NONE;
        }
    } 
    
    if(merging) {
        // if merging, update receptor, and then do 'merge' animation
        mReceptor->update(level, delta);
    }
}

void Movable::render(SDL_Renderer* renderer) const {
    // render receptor first when merging
    if(merging) {
        mReceptor->render(renderer);
    } else if(boostPower > 0 && booster.get()) {
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
        moveDir = DIR_NONE;
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
    endX = startX + xPosChange;
    endY = startY + yPosChange;

    // Update pos. of ptr in the level grid
    level->moveGridElement(gridX, gridY, newGridX, newGridY);

    // if merging with receptor, flip new tile (that entity just moved to)
    if(merging) level->flipMapTiles(gridX, gridY, parity);
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
        } else if(boostPower > 0) {
            // Check for another boost at next tile when already boosted
            // if there is, avoid boosting 2x in that direction
            if(!checkBoost(level, moveDir)) {
                initMovement(moveDir, level);
                boostPower--;
            }
        } else {
            moveDir = DIR_NONE;
            moving = false;

            if(booster.get()) booster.reset();
        }
    }
}

// check for a boost in the specified direction, and interact accordingly
bool Movable::checkBoost(Level * level, Direction direction) {
    auto boost = getEntity<Boost>(level, direction);
    
    // if there is a boost in the tile we want to move to, activate it
    if(boost.get()) {
        // remove booster from map/store
        booster = boost;
        level->removeGridElement(boost->getGridX(), boost->getGridY());

        boost->setActivated(true);

        // if currently boosted/set to move, finish move
        if(boostPower > 0 || moveDir != DIR_NONE) { 
            initMovement(moveDir, level);    
        }

        // store boost power and direction, overwriting any existing boosts
        boostPower = boost->getPower();
        moveDir = boost->getDirection();
        
        return true;
    }

    return false;
}


// handle interaction between a movable entity/its receptor
void Movable::checkReceptor(Level * level) {
    auto receptor = getEntity<Receptor>(level, currCheckDir());

    // check that receptor is not yet completed + has the correct shape
    if(receptor.get() && !receptor->isCompleted() && receptor->getShape() == movableShape) {
        merging = true;
        receptor->setCompleted(true);

        // remove receptor from grid and track receptor from this entity
        mReceptor = receptor;
        level->removeGridElement(receptor->getGridX(), receptor->getGridY());

        // stop movement after next move if boosting
        boostPower = boostPower > 0 ? 1 : 0;
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

Direction Movable::currCheckDir() const {
    Direction currCheckDir = (moving && boostPower == 0) ? bufferedDir : moveDir;
    return currCheckDir;
}

float Movable::getMoveProg() const {
    return moveProg; 
}

bool Movable::isMerging() const {
    return merging;
}

bool Movable::isMoving() const {
    return moving;
}