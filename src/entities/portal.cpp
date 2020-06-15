#include "entities/portal.hpp"
#include "entities/player.hpp"

Portal::Portal(int screenX, int screenY, int gridX, int gridY, int parity,
    std::shared_ptr<Sprite> entitySprite) : Entity(screenX, screenY, gridX,
    gridY, parity, entitySprite) {}

void Portal::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Portal::update(Level * level, float delta) {
    // if activated, handle player updates
    if(activated && player.get()) {
        // if player is done moving onto the portal, proceed with the teleport
        if(!player->isMoving() && player->isTeleporting()) {
            teleportPlayer(level);
            player->setTeleporting(false);
        }

        // if player has moved off of the portal(s), place them back in the grid
        int playerX = player->getGridX();
        int playerY = player->getGridY();
        if(!(playerX == gridX && playerY == gridY) && !(playerX == 
            otherPortal->getGridX() && playerY == otherPortal->getGridY())) {
            
            level->placePortals();

            // reset portal status/player pointer
            player.reset();
            setActivated(false);
            setRemoved(false);
        }
    }
}

void Portal::render(SDL_Renderer* renderer) const {
    Entity::render(renderer);
}

// teleport the player to the other portal, placing it back in the grid
void Portal::teleportPlayer(Level * level) {
    level->moveGridElement(player->getGridX(), player->getGridY(),
        otherPortal->getGridX(), otherPortal->getGridY());

    // update player render position
    player->setScreenX(otherPortal->getScreenX());
    player->setScreenY(otherPortal->getScreenY());
}


// temporarily lift portals from grid
void Portal::removePortals(Level * level) {
    level->removeGridElement(gridX, gridY);
    level->removeGridElement(otherPortal->getGridX(), otherPortal->getGridY());

    setRemoved(true);
}

void Portal::setPlayer(std::shared_ptr<Player> player) {
    this->player = player;
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