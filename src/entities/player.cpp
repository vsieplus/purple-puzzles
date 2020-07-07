// Implementation for Player class

#include <stdlib.h>

#include "entities/player.hpp"
#include "entities/receptor.hpp"
#include "entities/diamond.hpp"
#include "entities/portal.hpp"
#include "level/level.hpp"

Player::Player(int screenX, int screenY, int gridX, int gridY, int parity,
    std::shared_ptr<Sprite> entitySprite,
    const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations) : 
    Movable(screenX, screenY, gridX, gridY, PLAYER_VELOCITY, parity,
    entitySprite, PLAYER_SHAPE, entityAnimations) {}

void Player::handleEvents(const Uint8 * keyStates, Level * level) {
    // check for move undo ('u')
    if(keyStates[SDL_SCANCODE_U] && undoBuffer == 0) {
        undoBuffer = UNDO_BUFFER_CAP;
        movesUndone++;
    } else if((!moving || (moveProg > MOVEMENT_BUFFER && bufferedDir == DIR_NONE)) &&
        boostPower == 0 && !merging && !teleporting) {

        // Check if player wants to start moving or buffer a move, when not boosted,
        // merging, or teleporting
        checkMovement(keyStates, level);
    }
}

void Player::update(Level * level, float delta) {
    // check for move undo
    if(undoBuffer > 0 && undoBuffer-- == UNDO_BUFFER_CAP) {
        if(level->isPerfect()) level->setPerfect(false);
        undoAction(level);
    } else if(moveDir != DIR_NONE || bufferedDir != DIR_NONE) {
        // signal last portal to check surrounded upon move if not yet vanished   
        if(lastPortal.get() && !lastPortal->isVanished() && 
            !(gridX == lastPortal->getGridX() && gridY == lastPortal->getGridY())) {
            lastPortal->checkSurrounded(level);
        }

        // check for entity interaction if player tried to move/is moving
        pushDiamond(level);
        checkReceptor(level);
        checkPortal(level);

        auto newCoords = getCoords(currCheckDir());

        // check for failed move
        if(!moving && !entityAnimator.isAnimating() && 
            (!level->inBounds(newCoords.first, newCoords.second) || 
            parity == level->getTileParity(newCoords.first, newCoords.second))) {
            switch(currCheckDir()) {
                case DIR_LEFT:  activateAnimation(PLAYER_MOVEFAIL_LEFT);
                                break;
                case DIR_RIGHT: activateAnimation(PLAYER_MOVEFAIL_RIGHT);
                                break;
                case DIR_UP:    activateAnimation(PLAYER_MOVEFAIL_UP);
                                break;
                case DIR_DOWN:  activateAnimation(PLAYER_MOVEFAIL_DOWN);
                                break;
                default:        break;
            }

            level->playSound(BONK_SOUND_ID);
        }
    } else if(merging) {
        // if player not moving + is merging, check if level is complete
        if(!moving && !level->isCompleted()) {
            if(!vanished) {
                activateAnimation(PLAYER_MERGE);
                vanished = true;
            } else if(!entityAnimator.isAnimating() && !stuck) {
                stuck = !level->checkComplete();
            }
        }
    }

    // update player movement
    Movable::update(level, delta);
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
}

void Player::pushDiamond(Level * level) {
    Direction pushDir = currCheckDir();
    auto diamond = getEntity<Diamond>(level, pushDir);

    // set move direction of diamond if not already moving or merging/merged w/receptor
    if(diamond.get() && !diamond->isMoving() && !diamond->isMerging()) {
        auto newDCoords = diamond->getCoords(pushDir);
        
        // set the move direction of the diamond
        diamond->setMoveDir(pushDir);
        
        // add push only if diamond has a legal move
        auto diamondReceptor = diamond->getEntity<Receptor>(level, pushDir);

        if((!diamond->checkCollision(level, newDCoords.first, newDCoords.second) &&
            diamond->getParity() != level->getTileParity(newDCoords.first, newDCoords.second)) ||
            diamondReceptor.get() != nullptr) {
            
            pushedObjects.push(diamond);
            actionHistory.push(Movable::MovableAction::PUSH);   
        }
    }
}

void Player::checkPortal(Level * level) {
    auto portal = getEntity<Portal>(level, currCheckDir());

    // check if portal is there, if so, activate teleport status
    if(portal.get() && !portal->isVanished()) {
        teleporting = true;

        portal->setActivated(true);

        // set portal's player, remove portals from grid temporarily
        portal->setPlayer(level->getGridElement<Player>(gridX, gridY));
        portal->removePortals(level);

        // store portal for undo purposes
        lastPortal = portal;

        // reduce boosting if currently boosted
        if(boostPower > 1) boostPower = 1;
    }
}

void Player::undoAction(Level * level) {
    // handle undo for player-specific actions
    if(!actionHistory.empty()) {
        MovableAction lastAction = actionHistory.top();

        switch(lastAction) {
            // for push actions, simply undo the last action of the last pushed obj.
            case PUSH:
                // undo the last action of the pushed object
                pushedObjects.top()->undoAction(level);
                pushedObjects.pop();

                // pop the PUSH action from this player's history
                actionHistory.pop();
                break;

            // if teleport was last action, player must still be on portal
            // -> portal is still tracking player, so teleport them back
            case TELEPORT:
                lastPortal->setVanished(false);
                lastPortal->teleportPlayer(level, true);
                actionHistory.pop();

                // undo move onto portal
                undoAction(level);

                // give ownership of portals -> level
                level->placePortals();
                break;
            case MERGE:
                stuck = false;
            default:
                // check for undoing general movement
                Movable::undoAction(level);
                break;
        }
    }
}

void Player::pushAction(MovableAction action) {
    actionHistory.push(action);
}

bool Player::isTeleporting() const {
    return teleporting;
}

void Player::setTeleporting(bool teleporting) {
    this->teleporting = teleporting;
}

int Player::getMovesUndone() const {
    return movesUndone;
}


void Player::setLastPortal(std::shared_ptr<Portal> lastPortal) {
    this->lastPortal = lastPortal;
}