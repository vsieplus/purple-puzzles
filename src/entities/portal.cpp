#include <array>

#include "entities/portal.hpp"
#include "entities/player.hpp"

Portal::Portal(int screenX, int screenY, int gridX, int gridY, int parity,
    std::shared_ptr<Sprite> entitySprite,
    const std::unordered_map<int, std::shared_ptr<Animation>> & entityAnimations) : 
    Entity(screenX, screenY, gridX, gridY, parity, entitySprite, entityAnimations) {}

void Portal::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Portal::update(Level * level, float delta) {
    // if activated, handle player updates
    if(activated && player.get()) {
        // if player is done moving onto the portal, proceed with the teleport
        if(!player->isMoving() && player->isTeleporting()) {
            teleportPlayer(level);
            player->setTeleporting(false);

            // transfer ownership of player to otherPortal
            player->setLastPortal(otherPortal);
            otherPortal->setPlayer(player);
            player.reset();
            checkSurrounded(level);
            return;
        }

        // if player has moved off of the portal, place them back in the grid
        if(!player->isMoving() && !(player->getGridX() == gridX && player->getGridY() == gridY)) {
            
            // give ownership back to level
            level->placePortals();
            checkSurrounded(level, false);

            resetPortalStatus(level);
        }
    }

    Entity::update(level, delta);
}

void Portal::render(SDL_Renderer* renderer) const {
    Entity::render(renderer);
}

// transfer ownership of portals back to level/reset active status
void Portal::resetPortalStatus(Level * level) {
    // reset portal status/player pointer
    setActivated(false);
    setRemoved(false);
    player.reset();
}

// check if a portal is surrounded by purple tiles, if so -> vanish
void Portal::checkSurrounded(Level * level, bool playerTeleported) {
    if(vanished) return;

    std::array<std::pair<int, int>, 4> coords;
    coords[0] = getCoords(DIR_UP);
    coords[1] = getCoords(DIR_DOWN);
    coords[2] = getCoords(DIR_LEFT);
    coords[3] = getCoords(DIR_RIGHT);

    // check if each is a purple tile    
    for(int i = 0; i < 4; i++) {
        if(level->getTileParity(coords[i].first, coords[i].second) != PARITY_PURPLE) {
            // if not purple, may skip if player is on same tile
            if(!playerTeleported &&  player->getCoords(DIR_NONE) == coords[i]) continue;

            // otherwise early return b/c some surrounding tile not purple
            return;
        }
    }

    // if made it here, considered 'surrounded' -> activate vanish anim.
    activateAnimation(PORTAL_MERGE);
    vanished = true;

    // remove from entity grid + flip tile
    level->removeGridElement(gridX, gridY);
    level->flipMapTiles(gridX, gridY, parity);
}

// teleport the player to the other portal, placing it back in the grid
void Portal::teleportPlayer(Level * level, bool undo) {
    level->moveGridElement(player->getGridX(), player->getGridY(),
        otherPortal->getGridX(), otherPortal->getGridY());

    // update player render position
    player->setScreenX(otherPortal->getScreenX());
    player->setScreenY(otherPortal->getScreenY());

    // add a teleport to the player's action history if not undoing
    if(!undo) {
        player->pushAction(Movable::MovableAction::TELEPORT);
    } else {
        // otherwise, place the portals back/transfer back player ownership
        player->setLastPortal(otherPortal);
        otherPortal->setVanished(false);
        otherPortal->setPlayer(player);
        resetPortalStatus(level);

        // undo potential tileflip
        if(level->getTileParity(otherPortal->getGridX(), otherPortal->getGridY()) == PARITY_PURPLE) {
            level->flipMapTiles(otherPortal->getGridX(), otherPortal->getGridY(), PARITY_GRAY, undo);
        }
    }
}

// temporarily lift portals from grid
void Portal::removePortals(Level * level) {
    level->removeGridElement(gridX, gridY);
    level->removeGridElement(otherPortal->getGridX(), otherPortal->getGridY());

    setRemoved(true);
}

// set player of this portal, from which they will teleport to the otherPortal (whose player is unset)
void Portal::setPlayer(std::shared_ptr<Player> player) {
    this->player = player;
}

std::shared_ptr<Player> Portal::getPlayer() const {
    return player;
}

void Portal::setOtherPortal(std::shared_ptr<Portal> otherPortal) {
    this->otherPortal = otherPortal;
}

void Portal::setRemoved(bool removed) {
    this->removed = removed;
    if(otherPortal->isRemoved() != removed) {
        otherPortal->setRemoved(removed);
    } 
}

bool Portal::isRemoved() const {
    return removed;
}

// maintain activated status for both portals
void Portal::setActivated(bool activated) {
    this->activated = activated;
    if(otherPortal->isActivated() != activated) {
        otherPortal->setActivated(activated);
    }
}

bool Portal::isActivated() const {
    return activated;
}