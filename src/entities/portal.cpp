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
            return;
        }

        // if player has moved off of the portal, place them back in the grid
        if(!(player->getGridX() == gridX && player->getGridY() == gridY)) {
            
            // give ownership back to level
            level->placePortals();

            resetPortalStatus(level);
        }
    }

    Entity::update(level, delta);
}

// transfer ownership of portals back to level/reset active status
void Portal::resetPortalStatus(Level * level) {
    // reset portal status/player pointer
    setActivated(false);
    setRemoved(false);
    player.reset();
}

void Portal::render(SDL_Renderer* renderer) const {
    Entity::render(renderer);
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
        otherPortal->setPlayer(player);
        resetPortalStatus(level);
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