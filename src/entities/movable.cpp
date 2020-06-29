#include "entities/movable.hpp"
#include "level/level.hpp"

Movable::Movable(int screenX, int screenY, int gridX, int gridY, int velocity,
    int parity, std::shared_ptr<Sprite> entitySprite, std::string movableShape,
    const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations) : 
    Entity(screenX, screenY, gridX, gridY, parity, entitySprite, entityAnimations),
    startX(screenX), startY(screenY), endX(screenX), endY(screenY), velocity(velocity), 
    movableShape(movableShape) {}

void Movable::update(Level * level, float delta) {  
    if(moving) {
        // if boosted/replacing booster, update booster
        if(boostPower > 0) {
            if(!boosters.empty()) {
                boosters.top()->update(level, delta);
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

    // update animation
    Entity::update(level, delta);
}

void Movable::render(SDL_Renderer* renderer) const {
    // render receptor first when merging/booster when boosting
    if(merging) {
        mReceptor->render(renderer);
    } else if(boostPower > 0) {
        if(!boosters.empty()) {
            boosters.top()->render(renderer);
        }
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
 *        position. Returns early if invalid
 */
void Movable::initMovement(int xPosChange, int yPosChange, int xGridChange, 
    int yGridChange, Direction direction, Level * level) {

    int newGridX = gridX + xGridChange;
    int newGridY = gridY + yGridChange;

    // Check for collisions or invalid tile movement (same tile Parity)
    if(checkCollision(level, newGridX, newGridY) || parity == level->getTileParity(newGridX, newGridY)) {
        
        // if failing on a boost, add a dummy "boost-in-place" to action hist.
        if(boostPower > 0) {
            addMoveToHistory(DIR_NONE);
        }

        moveDir = DIR_NONE;
        moving = false;
        boostPower = 0;
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

    addMoveToHistory(direction);
}

// add an entity movement to its history
void Movable::addMoveToHistory(Direction direction) {
    MovableAction moveAction;

    bool boosted = boostPower > 0;

    // add a normal move or boosted move to the stack
    switch(direction) {
        case DIR_UP:    moveAction = boosted ? BOOST_UP : MOVE_UP;
                        break;
        case DIR_DOWN:  moveAction = boosted ? BOOST_DOWN : MOVE_DOWN;
                        break;
        case DIR_LEFT:  moveAction = boosted ? BOOST_LEFT : MOVE_LEFT;
                        break;
        case DIR_RIGHT: moveAction = boosted ? BOOST_RIGHT : MOVE_RIGHT;
                        break;
        case DIR_NONE:  moveAction = BOOST_NONE;
                        break;                                                                  
    }

    actionHistory.push(moveAction);
}

// Move the player
void Movable::move(Level * level, float delta) {
    if(moveProg < 1.0f) {
        // Update moveProg based on time;
        moveProg += velocity * (delta/1000.f);

        // Perform linear interpolation if not yet reached
        std::pair<int, int> newPos = lerp(startX, startY, endX, endY, moveProg);

        // For possible overshoot from float error, set to end position
        renderArea.x = abs(newPos.first - startX) < abs(endX - startX) ? newPos.first : endX;
        renderArea.y = abs(newPos.second - startY) < abs(endY - startY) ? newPos.second : endY;
    } else {
        // When current move is finished check if a move is buffered/boosting
        if(boostPower > 0) {
            // Check for another boost at next tile when already boosted
            // if there is, avoid boosting 2x in that direction
            if(!checkBoost(level, moveDir)) {
                initMovement(moveDir, level);
                
                // decrement boost power if init was succesful (we're now moving)
                if(moving) boostPower--;
            }
        } else if(bufferedDir != DIR_NONE) {
            // check for boost in the buffered direction, if there is one there
            // avoid intializing movement 2x
            if(!checkBoost(level, bufferedDir)) {
                initMovement(bufferedDir, level);
            }

            bufferedDir = DIR_NONE;
        } else {
            moveDir = DIR_NONE;
            moving = false;
        }
    }
}

// check for a boost in the specified direction, and interact accordingly
bool Movable::checkBoost(Level * level, Direction direction) {
    auto boost = getEntity<Boost>(level, direction);
    
    // if there is a boost in the tile we want to move to, activate it
    if(boost.get()) {
        // remove booster from map/store
        boosters.push(boost);
        level->removeGridElement(boost->getGridX(), boost->getGridY());

        boost->setActivated(true);

        // if currently boosted/set to move, finish move (to the boost tile)
        if(boostPower > 0 || moveDir != DIR_NONE || bufferedDir != DIR_NONE) { 
            initMovement(direction, level);    
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

        // add merge to action history
        actionHistory.push(MERGE);
    }
}

// undo the last action taken by this entity
void Movable::undoAction(Level * level) {
    if(!actionHistory.empty()) {
        // reset any movement/boosting
        moving = false;
        boostPower = 0;
        moveDir = DIR_NONE;
        bufferedDir = DIR_NONE;

        MovableAction lastAction = actionHistory.top();
        
        switch(lastAction) {
            // single movements
            case MOVE_LEFT:     undoMovement(DIR_LEFT, level);
                                break;
            case MOVE_RIGHT:    undoMovement(DIR_RIGHT, level);
                                break;
            case MOVE_DOWN:     undoMovement(DIR_DOWN, level);
                                break;
            case MOVE_UP:       undoMovement(DIR_UP, level);
                                break;
            // for boosted moves, undo 1 move, then recurse if applicable (^)
            case BOOST_LEFT:    undoBoost(DIR_LEFT, level, lastAction);
                                break;
            case BOOST_RIGHT:   undoBoost(DIR_RIGHT, level, lastAction);
                                break;
            case BOOST_DOWN:    undoBoost(DIR_DOWN, level, lastAction);
                                break;
            case BOOST_UP:      undoBoost(DIR_UP, level, lastAction);
                                break;
            case BOOST_NONE:    undoBoost(DIR_NONE, level, lastAction);
                                break;
            case MERGE:         undoMerge(level);
                                break;
            default:            break;
        }
    }
}

// undoes a move made originally in the specified direction
void Movable::undoMovement(Direction direction, Level * level) {
    // pop the movement/boost from the action history
    actionHistory.pop();

    std::pair<int, int> origCoords;

    switch(direction) {
        case DIR_UP:    origCoords = getCoords(DIR_DOWN);
                        break;
        case DIR_DOWN:  origCoords = getCoords(DIR_UP);
                        break;
        case DIR_LEFT:  origCoords = getCoords(DIR_RIGHT);
                        break;
        case DIR_RIGHT: origCoords = getCoords(DIR_LEFT);
                        break;
        case DIR_NONE:  return;
    }

    // undo tile flip
    level->flipMapTiles(origCoords.first, origCoords.second, PARITY_GRAY, true);

    // reset position
    setScreenX(origCoords.first * entitySprite->getWidth());
    setScreenY(origCoords.second * entitySprite->getHeight());

    // if previous move was a teleport, transfer ownership of portals back to level
    if(!actionHistory.empty() && actionHistory.top() == TELEPORT) {
        lastPortal->removePortals(level);
        lastPortal->setActivated(true);
        lastPortal->setPlayer(level->getPlayer());
    }

    // place the entity at its orig. position
    level->moveGridElement(gridX, gridY, origCoords.first, origCoords.second);

    // check if next most recent action on top is merge, if so undo
    if(!actionHistory.empty() && actionHistory.top() == MERGE) {
        undoMerge(level);
    }
}

// undo a boosted move
void Movable::undoBoost(Direction direction, Level * level, MovableAction lastAction) {
    // undo movement and pop the BOOST_... move
    undoMovement(direction, level);
    
    // check if after the undo the last boost's coords match our current, if so replace
    if(!boosters.empty()) {
        std::shared_ptr<Boost> lastBoost = boosters.top();

        // transfer ownership back to the map
        if(lastBoost->getGridX() == gridX && lastBoost->getGridY() == gridY) {
            boosters.pop();

            lastBoost->setActivated(false);
            lastBoost->setVanished(false);
            
            // undo the movement onto the boost
            undoAction(level);

            // update lastAction if there are still more actions after recursing
            if(!actionHistory.empty()) {
                lastAction = actionHistory.top();
            }
                
            // we have moved off the grid location, so we may now replace the boost here
            level->placeGridElement(lastBoost, lastBoost->getGridX(), lastBoost->getGridY());
        }
    }

    // check if the next top element is a boost
    bool topIsBoost = !actionHistory.empty() && (actionHistory.top() == BOOST_LEFT
        || actionHistory.top() == BOOST_RIGHT || actionHistory.top() == BOOST_DOWN
        || actionHistory.top() == BOOST_UP);

    bool lastBoost = !topIsBoost && (lastAction == BOOST_DOWN || lastAction == BOOST_UP
        || lastAction == BOOST_LEFT || lastAction == BOOST_RIGHT);

    // recurse for boosts if top is another boost or currently on the last move/boost onto a booster
    if(topIsBoost || lastBoost) {
        undoAction(level);
    }
}


// undo merge with receptor, and movement onto the receptor + ownership
void Movable::undoMerge(Level * level) {
    actionHistory.pop();

    // then undo the merge
    merging = false;
    mReceptor->setCompleted(false);

    level->flipMapTiles(mReceptor->getGridX(), mReceptor->getGridY(), PARITY_GRAY, true);
    level->placeGridElement(mReceptor, mReceptor->getGridX(), mReceptor->getGridY());
    mReceptor.reset();
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