// diamond implementation

#include "level/level.hpp"

#include "entities/diamond.hpp"
#include "entities/receptor.hpp"

const std::string Diamond::DIAMOND_SHAPE = "diamond";

Diamond::Diamond(int screenX, int screenY, int gridX, int gridY, int parity,
    std::shared_ptr<Sprite> entitySprite) : Movable(screenX,
    screenY, gridX, gridY, DIAMOND_VELOCITY, parity, entitySprite) {

}

void Diamond::handleEvents(const Uint8 * keyStates, Level * level) {

}

void Diamond::update(Level * level, float delta) {
    // if merging, update receptor, and then do 'merge' animation
    if(merging) {
        dReceptor->update(level, delta);
    }

    // check for receptor
    if(moveDir != DIR_NONE) {
        checkReceptor(level, moveDir);
    }

    if(boostDir != DIR_NONE) {
        checkReceptor(level, boostDir);
    }
    
    // normal movement
    Movable::update(level, delta);
}

void Diamond::render(SDL_Renderer* renderer) const {
    // render receptor first when merging
    if(merging) {
        dReceptor->render(renderer);
    }
    
    Movable::render(renderer);
}

void Diamond::checkReceptor(Level * level, Direction direction) {
    std::pair<int, int> pushCoords = getCoords(direction);

    // check for receptor at tile we're going to check
    auto receptor = level->getGridElement<Receptor>(pushCoords.first,
        pushCoords.second);

    // check that receptor is not yet completed + has the correct shape
    if(receptor.get() && !receptor->isCompleted() && receptor->getShape() == DIAMOND_SHAPE) {
        merging = true;
        receptor->setCompleted(true);

        // remove receptor from grid and track receptor from this diamond
        dReceptor = receptor;
        level->removeGridElement(receptor->getGridX(), receptor->getGridY());

        // stop movement after next move
        boostPower = 1;
    }
}

bool Diamond::isMerging() const {
    return merging;
}